#include "Chunk.h"
#include "ChunkMesher.h"
#include "ChunkMesh.h"




namespace Engine{
   

    Chunk::Chunk(const ChunkCoord & Coord) : m_ChunkCoord(Coord){
        m_Blocks.fill(static_cast<uint16_t>(BlockType::AIR));
    }

    Chunk::~Chunk()
    {
    }

    void Chunk::SetBlock(int x, int y, int z, BlockType block){
        m_Blocks[GetIndex(x,y,z)] = static_cast<uint16_t>(block);
    }

    void Chunk::GenerateFlatChunk(){
        for(int y = 0;y < HEIGHT;y++){
            for(int x = 0;x < WIDTH;x++){
                for(int z = 0;z < DEPTH;z++){
                    BlockType type;
                    if (y > 19 && y < 21) {         
                        type = BlockType::GRASS;
                    } else if (y > 15 && y <= 19) {  
                        type = BlockType::DIRT;
                    } else if (y <= 15) {            
                        type = BlockType::STONE;
                    } else {
                        type = BlockType::AIR;      
                    }
                    SetBlock(x, y, z, type);
                }
            }
        }
        // SetBlock(1,25,1,BlockType::GRASS);
        // SetBlock(0,24,0,BlockType::DIRT);
        SetBlock(2,26,2,BlockType::STONE);
        // SetBlock(3,27,3,BlockType::SAND);
    }

    BlockType Chunk::GetBlockType(int x, int y, int z) const{
        return static_cast<BlockType>(m_Blocks[GetIndex(x,y,z)]);
    }

    void Chunk::RebuildMesh(){
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        ChunkMesher::GenerateMesh(*this, vertices, indices);
        if (!m_Mesh) m_Mesh = std::make_unique<ChunkMesh>();
        m_Mesh->Upload(vertices, indices);
        m_IsMeshDirty = false;
    }

    void Chunk::Render(){
        if(m_Mesh && m_Mesh->IsValid()){
            m_Mesh->Draw();
        }
    }

    void Chunk::UploadMesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices){
        if(!m_Mesh){
            m_Mesh = std::make_unique<ChunkMesh>();
        }
        m_Mesh->Upload(vertices,indices);
        m_IsMeshDirty = true;
    }

    void Chunk::Render(Shader &shader){
        if(m_Mesh && m_Mesh->IsValid()){
            m_Mesh->Draw();
        }
    }

    /*
    const BlockAttribution &Chunk::GetBlockAttribution(int x, int y, int z) const {
       return BlockRegistry::Get().GetAttribution(static_cast<BlockType>(m_Blocks[GetIndex(x,y,z)]));
    }
    */
}