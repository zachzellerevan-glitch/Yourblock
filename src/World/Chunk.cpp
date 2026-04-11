#include "Chunk.h"

namespace Engine{
    Chunk::Chunk(int chunkx, int chunkz) : m_ChunkX(chunkx),m_ChunkZ(chunkz){
        m_Blocks.fill(static_cast<uint16_t>(BlockType::AIR));
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
                    } else if (y > 15 && y < 19) {  
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
    }

    BlockType Chunk::GetBlockType(int x, int y, int z) const{
        return static_cast<BlockType>(m_Blocks[GetIndex(x,y,z)]);
    }

    /*
    const BlockAttribution &Chunk::GetBlockAttribution(int x, int y, int z) const {
       return BlockRegistry::Get().GetAttribution(static_cast<BlockType>(m_Blocks[GetIndex(x,y,z)]));
    }
    */
}