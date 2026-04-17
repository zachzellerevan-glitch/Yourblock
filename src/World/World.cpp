#include "World.h"

namespace Engine{
    World::World(int LoadRadius) : m_LoadRadius(LoadRadius){
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
        ChunkCoord PlayerChunk;
        PlayerChunk.x = static_cast<int>(std::floor(PlayerPos.x / Chunk::WIDTH)); //Chunk index start from 0 so floor
        PlayerChunk.z = static_cast<int>(std::floor(PlayerPos.z / Chunk::DEPTH));

        for(int dx = -m_LoadRadius;dx <= m_LoadRadius;dx++){
            for(int dz = -m_LoadRadius;dz <= m_LoadRadius;dz++){
                ChunkCoord LoadedChunk;
                LoadedChunk.x = PlayerChunk.x + dx;
                LoadedChunk.z = PlayerChunk.z + dz;
                if(!GetChunk(LoadedChunk)){
                    LoadChunks(LoadedChunk);
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
    }
    
    void World::Render(Shader &shader){
        for(auto & [coords,chunks] : m_ChunkHash){
            glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(coords.x * Chunk::WIDTH,0.0f,coords.z * Chunk::DEPTH));
            shader.SetUniform("u_Model",model);
            chunks->Render(shader);
        }
    }
    
    Chunk *World::GetChunk(const ChunkCoord & Coord){
        auto it = m_ChunkHash.find(Coord);
        return it != m_ChunkHash.end() ? it->second.get() : nullptr;
    }
    
    const Chunk *World::GetChunk(const ChunkCoord &Coord) const{
        auto it = m_ChunkHash.find(Coord);
        return it != m_ChunkHash.end() ? it->second.get() : nullptr;
    }
}