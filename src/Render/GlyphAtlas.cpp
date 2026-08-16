#include "GlyphAtlas.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>      // GL 函数
#include <cstring>          // memcpy
#include <algorithm>        // std::max
#include <iostream>         // std::cerr(YBpch 若已带可省)
#include <unordered_set>

namespace Engine{
    namespace {
        std::vector<unsigned char> MakeRGBA(const FT_Bitmap & bmp){
        std::vector<unsigned char> out(bmp.width * bmp.rows * 4);
        for(int i = 0; i < bmp.width * bmp.rows; i++){
            out[i*4+0] = 255; out[i*4+1] = 255; out[i*4+2] = 255;
            out[i*4+3] = bmp.buffer[i];          // alpha = 覆盖度
        }
        return out;
    }


        struct RawGlyph {                      // 光栅化结果临时容器
            std::vector<unsigned char> rgba;
            int w = 0, h = 0;
            glm::vec2 bearing;
            float advance = 0.0f;
        };

        void CollectCodepoints(const char* text, std::unordered_set<unsigned int>& out){
            const char* p = text;
            while(*p) out.insert(DecodeUTF8(p));   // 顺便去重
        }
    }

    void GlyphAtlas::Build(const char* fontPath, int pixelSize){
        if(m_Built) return;

        FT_Library lib;
        if(FT_Init_FreeType(&lib)){ std::cerr << "FT_Init failed\n"; return; }
        FT_Face face;
        if(FT_New_Face(lib, fontPath, 0, &face)){   // 字体路径错会走到这
            std::cerr << "FT_New_Face failed: " << fontPath << "\n";
            FT_Done_FreeType(lib); return;
        }
        FT_Set_Pixel_Sizes(face, 0, pixelSize);
        m_PixelSize = pixelSize;

        // ① 光栅化:这次真的存进 raws
        std::unordered_set<unsigned int> cps;
        for(unsigned int cp = 32; cp <= 126; cp++) cps.insert(cp);
        CollectCodepoints("你好世界 暂停 设置 保存 加载 返回 重新开始", cps);

        std::unordered_map<unsigned int, RawGlyph> raws;
        for(unsigned int cp : cps){
            if(FT_Load_Char(face, cp, FT_LOAD_RENDER)) continue;   // 字体里没有就跳过
            FT_GlyphSlot g = face->glyph;
            raws[cp] = { MakeRGBA(g->bitmap),
                        (int)g->bitmap.width, (int)g->bitmap.rows,
                        {(float)g->bitmap_left, (float)g->bitmap_top},
                        (float)g->advance.x / 64.0f };
        }

        // ② 行打包进 512×512
        const int atlasW = 512, atlasH = 512;
        std::vector<unsigned char> atlas(atlasW * atlasH * 4, 0);
        int penX = 0, penY = 0, rowH = 0;
        for(auto & [cp, raw] : raws){
            if(raw.w == 0 || raw.h == 0){                // 空格:不占图集,只留 advance
                m_Glyphs[cp] = {{0,0,0,0}, {0,0}, raw.bearing, raw.advance};
                continue;
            }
            if(penX + raw.w > atlasW){ penX = 0; penY += rowH; rowH = 0; }
            for(int r = 0; r < raw.h; r++)               // 逐行拷进图集
                std::memcpy(&atlas[((penY + r) * atlasW + penX) * 4],
                            &raw.rgba[r * raw.w * 4], raw.w * 4);

            m_Glyphs[cp] = {
                {(float)penX/atlasW, (float)penY/atlasH,
                 (float)(penX+raw.w)/atlasW, (float)(penY+raw.h)/atlasH},
                {(float)raw.w, (float)raw.h},
                raw.bearing, raw.advance
            };
            penX += raw.w + 1;                           // 1px 间隙防 GL_LINEAR 串色
            rowH = std::max(rowH, raw.h);
        }

        // ③ GL 纹理(和 UIIconAtlas 同一套,过滤用 GL_LINEAR)
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, atlasW, atlasH, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, atlas.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(lib);
        m_Built = true;
    }

    const Glyph & GlyphAtlas::GetGlyph(unsigned int codepoint) const{
        auto it = m_Glyphs.find(codepoint);
        if(it != m_Glyphs.end()) return it->second;
        static const Glyph fallback = {{0,0,0,0},{0,0},{0,0},0};  // 没烘焙的字 → 空字形,不崩
        return fallback;
    }

    unsigned int GlyphAtlas::GetTextureID() const{ return m_TextureID; }

    unsigned int DecodeUTF8(const char*& s){
        unsigned char c = (unsigned char)*s;          // 必须转 unsigned,否则中文负值全进 else
        if(c < 0x80){ s++; return c; }                // ASCII

        int n; unsigned int cp;
        if((c & 0xE0) == 0xC0){ n = 2; cp = c & 0x1F; }       // 2 字节
        else if((c & 0xF0) == 0xE0){ n = 3; cp = c & 0x0F; }  // 3 字节(中文在这)
        else if((c & 0xF8) == 0xF0){ n = 4; cp = c & 0x07; }  // 4 字节
        else { s++; return '?'; }                             // 非法首字节

        for(int i = 1; i < n; i++){
            unsigned char cont = (unsigned char)s[i];
            if((cont & 0xC0) != 0x80){ s++; return '?'; }     // 续字节必须是 10xxxxxx
            cp = (cp << 6) | (cont & 0x3F);
        }
        s += n;
        return cp;
    }
}