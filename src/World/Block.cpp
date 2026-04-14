#include "Block.h"

namespace Engine{
    const BlockAttribution &BlockRegistry::GetAttribution(BlockType block) const{
        return m_BlockAttributions[static_cast<int>(block)];
    }

    void BlockRegistry::LoadBlockConfig(std::string &ConfigPath){

    }

    BlockRegistry::BlockRegistry(){
        auto &TexArray = TextureArray::Get();
        
        TextureArray::Get().LoadTexture("assets/Texture");

        int Grass_Top = TexArray.GetLayer("grass_block_top");
        int Grass_Side = TexArray.GetLayer("grass_block_side");
        int Grass_Bottom = TexArray.GetLayer("dirt");
        int Dirt = TexArray.GetLayer("dirt");
        int Stone = TexArray.GetLayer("stone");
        int Sand = TexArray.GetLayer("sand");

        m_BlockAttributions[static_cast<int>(BlockType::AIR)] = {
            false,true,0,0,0,0,false
        };

        m_BlockAttributions[static_cast<int>(BlockType::DIRT)] = {
            true,false,1,Dirt,Dirt,Dirt,false
        };

        m_BlockAttributions[static_cast<int>(BlockType::GRASS)] = {
            true,false,1,Grass_Top,Grass_Side,Grass_Bottom,false
        };

        m_BlockAttributions[static_cast<int>(BlockType::STONE)] = {
            true,false,2,Stone,Stone,Stone,false
        };

        m_BlockAttributions[static_cast<int>(BlockType::SAND)] = {
            true,false,1,Sand,Sand,Sand,false
        };
        //TODO:texture in config
    }
}