#include "World.h"

namespace Engine{
    World::World(int LoadRadius,int ThreadCount) : m_LoadRadius(LoadRadius),m_ThreadCount(ThreadCount){
        m_Threadpool = std::make_unique<ThreadPool>(m_ThreadCount);
    }
    
    void World::LoadChunks(const ChunkCoord &Coord){
        //TO DO:async
        if(GetChunk(Coord)) return;
        auto chunk = std::make_unique<Chunk>(Coord);
        chunk->GenerateFlatChunk();
        chunk->RebuildMesh();
        m_ChunkHash[Coord] = std::move(chunk);

    }

    void World::UnloadChunks(const ChunkCoord &Coord){
        auto it = m_ChunkHash.find(Coord);
        if(it != m_ChunkHash.end()){
            m_ChunkHash.erase(it);
        }
    }

    void World::Update(const glm::vec3 &PlayerPos){
        ProcessCompleteChunk();
       
        ChunkCoord PlayerChunk;
        PlayerChunk.x = static_cast<int>(std::floor(PlayerPos.x / Chunk::WIDTH)); //Chunk index start from 0 so floor
        PlayerChunk.z = static_cast<int>(std::floor(PlayerPos.z / Chunk::DEPTH));

        for(int dx = -m_LoadRadius;dx <= m_LoadRadius;dx++){
            for(int dz = -m_LoadRadius;dz <= m_LoadRadius;dz++){
                ChunkCoord LoadedChunk;
                LoadedChunk.x = PlayerChunk.x + dx;
                LoadedChunk.z = PlayerChunk.z + dz;
                if(!GetChunk(LoadedChunk)){
                    GenerateChunkData(LoadedChunk);
                }
            }
        }

        for(auto it = m_ChunkHash.begin();it != m_ChunkHash.end();){
            int dx = std::abs(it->first.x - PlayerChunk.x);
            int dz = std::abs(it->first.z - PlayerChunk.z);
            if(dx > m_LoadRadius + 2 || dz > m_LoadRadius + 2){
                ChunkCoord CurrentUnload = it->first;
                it++;
                UnloadChunks(CurrentUnload);
            }else{
                it++;
            }
        }


        for(auto & [coord,chunk] : m_ChunkHash){
            if(chunk->GetDirty())
                chunk->RebuildMesh();
        }
    }
    
    void World::Render(Shader &shader){
        for(auto & [coords,chunks] : m_ChunkHash){
            glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(coords.x * Chunk::WIDTH,0.0f,coords.z * Chunk::DEPTH));
            shader.SetUniform("u_Model",model);
            chunks->Render(shader);
        }
    }

    BlockType World::GetBlock(int WorldX, int WorldY, int WorldZ) const{
        int inChunkX,inChunkZ;
        ChunkCoord cCoord;
        cCoord.x = WorldX >= 0 ? WorldX / Chunk::WIDTH : (WorldX + 1) / Chunk::WIDTH - 1;
        cCoord.z = WorldZ >= 0 ? WorldZ / Chunk::DEPTH : (WorldZ + 1) / Chunk::DEPTH - 1;

        inChunkX = WorldX - cCoord.x * Chunk::WIDTH;
        inChunkZ = WorldZ - cCoord.z * Chunk::DEPTH; 
        if(inChunkX < 0) inChunkX += Chunk::WIDTH;
        if(inChunkZ < 0) inChunkZ += Chunk::DEPTH;

        if(WorldY > Chunk::HEIGHT) return BlockType::AIR;

        auto chunk = GetChunk(cCoord);
        if(!chunk) return BlockType::AIR;
        return chunk->GetBlockType(inChunkX,WorldY,inChunkZ);
    }

    void World::SetBlock(int WorldX, int WorldY, int WorldZ, BlockType block){
        int inChunkX,inChunkZ;
        //Calculate Chunk coordinate
        ChunkCoord cCoord;
        cCoord.x = WorldX >= 0 ? WorldX / Chunk::WIDTH : (WorldX + 1) / Chunk::WIDTH - 1;
        cCoord.z = WorldZ >= 0 ? WorldZ / Chunk::DEPTH : (WorldZ + 1) / Chunk::DEPTH - 1;

        inChunkX = WorldX - cCoord.x * Chunk::WIDTH;
        inChunkZ = WorldZ - cCoord.z * Chunk::DEPTH; 
        if(inChunkX < 0) inChunkX += Chunk::WIDTH;
        if(inChunkZ < 0) inChunkZ += Chunk::DEPTH;

        if(WorldY > Chunk::HEIGHT) return;

        auto chunk = GetChunk(cCoord);
        chunk->SetBlock(inChunkX,WorldY,inChunkZ,block);

        chunk->SetDirty(true);
    }

    void World::SetBlock(glm::vec3 DestPos, BlockType block){
        int WorldX = std::floor(DestPos.x);
        int WorldY = std::floor(DestPos.y);
        int WorldZ = std::floor(DestPos.z);
        SetBlock(WorldX,WorldY,WorldZ,block);
    }

    Chunk *World::GetChunk(const ChunkCoord &Coord)
    {
        auto it = m_ChunkHash.find(Coord);
        return it != m_ChunkHash.end() ? it->second.get() : nullptr;
    }

    const Chunk *World::GetChunk(const ChunkCoord &Coord) const{
        auto it = m_ChunkHash.find(Coord);
        return it != m_ChunkHash.end() ? it->second.get() : nullptr;
    }

    void World::ProcessCompleteChunk(){
        std::queue<ChunkBuildResult> localQueue;
        {
            std::lock_guard<std::mutex> lock(m_CompleteMutex);
            std::swap(localQueue,m_CompleteQueue);
        }

        while(!localQueue.empty()){
            auto result = std::move(localQueue.front());
            localQueue.pop();
            {
                std::lock_guard<std::mutex> lock(m_LoadingMutex);
                m_LoadingSet.erase(result.coord);
            }
            auto & chunk = result.chunk;
            chunk->UploadMesh(std::move(result.vertices),std::move(result.indices));
            m_ChunkHash[result.coord] = std::move(chunk);

            // const int dx[6] = {1, -1, 0, 0, 0, 0};
            // const int dz[6] = {0, 0, 0, 0, 1, -1};
            // for (int i = 0; i < 6; ++i) {
            //     ChunkCoord neighbor;
            //     neighbor.x = result.coord.x + dx[i];
            //     neighbor.z = result.coord.z + dz[i];
            //     auto it = m_ChunkHash.find(neighbor);
            //     if (it != m_ChunkHash.end()) {
            //         it->second->SetDirty(true);
            //     }
            // }
        }

        
    }

    void World::GenerateChunkData(const ChunkCoord & Coord){
        {
            std::lock_guard<std::mutex> lock(m_LoadingMutex);
            if(m_LoadingSet.find(Coord) != m_LoadingSet.end()) return;
            m_LoadingSet.insert(Coord);
        }

        m_Threadpool->Submit([this,Coord](){
            auto chunk = std::make_unique<Chunk>(Coord);
            chunk->GenerateFlatChunk();
            
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            ChunkMesher::GenerateMesh(*chunk,vertices,indices,this);

            ChunkBuildResult result;
            result.coord = Coord;
            result.vertices = vertices;
            result.indices = indices;
            result.chunk = std::move(chunk);
            {
                std::lock_guard<std::mutex> lock(m_CompleteMutex);
                m_CompleteQueue.push(std::move(result));
            }
        });
    }
}