#pragma once

#include "YBpch.h"
#include "Render/TextureArray.h"
#include <glm/glm.hpp>

namespace Engine{ 
    enum class BlockType : uint16_t{ //65536 types
        AIR = 0,
        GRASS,DIRT,STONE,SAND
    }; 

    struct BlockAttribution{
        bool IsSolid;
        bool IsTransparent;
        float Hardness;
        int TextureTop;
        int TextureSide;
        int TextureBottom;
        bool IsLight;
    };

    enum class Face{
        PositiveX,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ
    };
    class BlockRegistry{
        public:
            static BlockRegistry & Get(){
                static BlockRegistry Instance;
                return Instance;
            }

            const BlockAttribution & GetAttribution(BlockType block) const;

            // 图集尺寸：256×256 像素（常见体素风格）。

            // 每个纹理格子尺寸：16×16 像素。

            // 网格排列：16 列 × 16 行，共 256 个格子。

            // 索引顺序：从左到右、从下到上（或者从上到下，取决于 UV 原点定义，但此处假定左下角为原点）。

            // 因此，每个格子占 UV 空间的 1/16 = 0.0625 单位（因为 UV 范围是 [0,1]）。


            inline std::array<float,4> GetUV(int textureIndex){
                float uMin = (textureIndex % 16) * 0.0625f; //u方向列号
                float vMin = (textureIndex / 16) * 0.0625f; //v方向行号
                float uMax = uMin + 0.0625f;
                float vMax = vMin + 0.0625f;
                return {uMin,vMin,uMax,vMax};
            }
            
            void LoadBlockConfig(std::string & ConfigPath);
        private:
            BlockRegistry();
            std::array<BlockAttribution,65536> m_BlockAttributions;            

    };
}