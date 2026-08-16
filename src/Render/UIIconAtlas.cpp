#include "UIIconAtlas.h"
#include "Render/TextureArray.h"
#include <glad/glad.h>

namespace Engine{
    namespace{   // 匿名命名空间:这个辅助函数只在本文件可见
        // 每个方块选一个面当图标:草用侧面(有草皮),其余用顶面
        int IconLayer(BlockType block){
            const auto & attr = BlockRegistry::Get().GetAttribution(block);
            return (block == BlockType::GRASS) ? attr.TextureSide : attr.TextureTop;
        }
    }
    void UIIconAtlas::Build(){
        if(m_Built) return;                       // 幂等:建过就直接返回
        constexpr int TILE = 16;
        constexpr int GRID = 3;                   // 3×3=48×48,够 9 种
        const auto & tex = TextureArray::Get();

        // ① 创建空的 2D 图集纹理
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GRID*TILE, GRID*TILE, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 像素风
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // ② 需要的方块(AIR 无图标)
        const BlockType blocks[] = { BlockType::GRASS, BlockType::DIRT,
                                     BlockType::STONE, BlockType::SAND };
        for(int i = 0; i < 4; i++){
            const auto & pixels = tex.GetCpuLayer(IconLayer(blocks[i]));
            if(pixels.empty()) continue;          // 层加载失败就跳过

            // ③ 填进第 i 格(坐标:列 i%3,行 i/3)
            const int tx = (i % GRID) * TILE;
            const int ty = (i / GRID) * TILE;
            glTexSubImage2D(GL_TEXTURE_2D, 0, tx, ty, TILE, TILE,
                            GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

            // ④ 记录 UV 矩形
            m_BlockUV[blocks[i]] = {
                (float)tx      / (GRID*TILE), (float)ty      / (GRID*TILE),
                (float)(tx+TILE) / (GRID*TILE), (float)(ty+TILE) / (GRID*TILE)
            };
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        m_Built = true;
    }

    glm::vec4 UIIconAtlas::GetUV(BlockType block) const{
        auto it = m_BlockUV.find(block);
        return it != m_BlockUV.end() ? it->second : glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    unsigned int UIIconAtlas::GetTextureID() const{ return m_TextureID; }
}