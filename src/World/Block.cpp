#include "Block.h"

namespace Engine{
    const BlockAttribution &BlockRegistry::GetAttribution(BlockType block) const{
        return m_BlockAttributions[static_cast<int>(block)];
    }

    void BlockRegistry::LoadBlockConfig(std::string &ConfigPath){

    }

    BlockRegistry::BlockRegistry()
    {
        auto MakeUV = [this](int texIndex){
            auto uv = GetUV(texIndex);
            return glm::vec4(uv[0],uv[1],uv[2],uv[3]);
        };

        constexpr int TEX_AIR = 0;
        constexpr int TEX_GRASS_TOP = 1;
        constexpr int TEX_GRASS_SIDE = 2;
        constexpr int TEX_GRASS_BOTTOM = 3;
        constexpr int TEX_DIRT = 3;
        constexpr int TEX_STONE = 4;
        constexpr int TEX_SAND = 5;

        m_BlockAttributions[static_cast<int>(BlockType::AIR)] = {
            false,true,0,MakeUV(TEX_AIR),MakeUV(TEX_AIR),MakeUV(TEX_AIR),false
        };
        m_BlockAttributions[static_cast<int>(BlockType::GRASS)] = {
            true,false,1,MakeUV(TEX_GRASS_TOP),MakeUV(TEX_GRASS_SIDE),MakeUV(TEX_GRASS_BOTTOM),false
        };
        m_BlockAttributions[static_cast<int>(BlockType::DIRT)] = {
            true,false,1,MakeUV(TEX_DIRT),MakeUV(TEX_DIRT),MakeUV(TEX_DIRT),false
        };
        m_BlockAttributions[static_cast<int>(BlockType::STONE)] = {
            true,false,2,MakeUV(TEX_STONE),MakeUV(TEX_STONE),MakeUV(TEX_STONE),false
        };
        m_BlockAttributions[static_cast<int>(BlockType::SAND)] = {
            true,false,1,MakeUV(TEX_SAND),MakeUV(TEX_SAND),MakeUV(TEX_SAND),false
        };
        //TODO:texture in config
    }
}