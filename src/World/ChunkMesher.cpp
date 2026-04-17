#include "ChunkMesher.h"
#include "Chunk.h"

namespace Engine{
    void ChunkMesher::GenerateMesh(const Chunk &chunk, std::vector<Vertex> &Verticies, std::vector<uint32_t> &Indicies) {
        Verticies.clear();
        Indicies.clear();

        Verticies.reserve(65536);
        Indicies.reserve(98304);

        for(int x = 0;x < Chunk::WIDTH;x++){
            for(int y = 0;y < Chunk::HEIGHT;y++){
                for(int z = 0;z < Chunk::DEPTH;z++){
                    const auto blocktype = chunk.GetBlockType(x,y,z);
                    if(blocktype == BlockType::AIR) continue;

                    const auto & Attribution = BlockRegistry::Get().GetAttribution(blocktype);
                    if(IsFaceVisible(chunk,x,y,z,Face::PositiveX)){
                        AddFace(Verticies,Indicies,x,y,z,Face::PositiveX,Attribution.TextureSide);
                    }
                    if(IsFaceVisible(chunk,x,y,z,Face::NegativeX)){
                     AddFace(Verticies,Indicies,x,y,z,Face::NegativeX,Attribution.TextureSide);
                    }
                    if(IsFaceVisible(chunk,x,y,z,Face::PositiveY)){
                     AddFace(Verticies,Indicies,x,y,z,Face::PositiveY,Attribution.TextureTop);
                    }
                    if(IsFaceVisible(chunk,x,y,z,Face::NegativeY)){
                     AddFace(Verticies,Indicies,x,y,z,Face::NegativeY,Attribution.TextureBottom);
                    }
                    if(IsFaceVisible(chunk,x,y,z,Face::PositiveZ)){
                     AddFace(Verticies,Indicies,x,y,z,Face::PositiveZ,Attribution.TextureSide);
                    }
                    if(IsFaceVisible(chunk,x,y,z,Face::NegativeZ)){
                        AddFace(Verticies,Indicies,x,y,z,Face::NegativeZ,Attribution.TextureSide);
                    }
                }
            }
        }
    }

    bool ChunkMesher::IsFaceVisible(const Chunk &chunk, int x, int y, int z, Face face){
        int nx = x,ny = y,nz = z;
        switch (face)
        {
        case Face::PositiveX:
            nx = x + 1;
            break;
        case Face::NegativeX:
            nx = x - 1;
            break;
        case Face::PositiveY:
            ny = y + 1;
            break;
        case Face::NegativeY:
            ny = y - 1;
            break;
        case Face::PositiveZ:
            nz = z + 1;
            break;
        case Face::NegativeZ:
            nz = z - 1;
            break;
        }

        if(nx < 0 || nx >= Chunk::WIDTH || nz < 0 || nz >= Chunk::DEPTH || ny < 0 || ny >= Chunk::HEIGHT){
            
        }

        const auto & Attribution = BlockRegistry::Get().GetAttribution(chunk.GetBlockType(nx,ny,nz));
        if(Attribution.IsTransparent){
            return true;
        }
        return false;
    }

    void ChunkMesher::AddFace(std::vector<Vertex> &Vertices, std::vector<uint32_t> &Indicies, int x, int y, int z, Face face,int Layer){
        uint32_t start = static_cast<uint32_t>(Vertices.size());
        const float uMin = 0.0f,uMax = 1.0f,vMin = 0.0f,vMax = 1.0f;

        glm::vec3 v0,v1,v2,v3;
        
        switch (face){
        case Face::PositiveX:
            v0 = {x + 1,y,z + 1};
            v1 = {x + 1,y,z};
            v2 = {x + 1,y + 1,z};
            v3 = {x + 1,y + 1,z + 1};
            break;
        case Face::NegativeX:
            v0 = {x,y,z};
            v1 = {x,y,z+1};
            v2 = {x,y+1,z+1};
            v3 = {x,y + 1,z};
            break;
        case Face::PositiveY:
            v0 = {x,y+1,z + 1};
            v1 = {x+1,y+1,z+1};
            v2 = {x+1,y+1,z};
            v3 = {x,y + 1,z};
            break;
        case Face::NegativeY:
            v0 = {x,y,z};
            v1 = {x+1,y,z};
            v2 = {x+1,y,z+1};
            v3 = {x,y,z+1};
            break;
        case Face::PositiveZ:
            v0 = {x,y,z+1};
            v1 = {x+1,y,z+1};
            v2 = {x+1,y+1,z+1};
            v3 = {x,y+1,z+1};
            break;
        case Face::NegativeZ:
            v0 = {x+1,y,z};
            v1 = {x,y,z};
            v2 = {x,y+1,z};
            v3 = {x+1,y+1,z};
            break;
        }

        Vertices.push_back({v0,{uMin,vMin},Layer});
        Vertices.push_back({v1,{uMax,vMin},Layer});
        Vertices.push_back({v2,{uMax,vMax},Layer});
        Vertices.push_back({v3,{uMin,vMax},Layer});

        Indicies.push_back(start);
        Indicies.push_back(start + 1);
        Indicies.push_back(start + 2);

        Indicies.push_back(start);
        Indicies.push_back(start + 2);
        Indicies.push_back(start + 3);
    }
    
    
}