#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

namespace Engine {
    struct Glyph {
        glm::vec4 UV;        // 图集 UV 矩形
        glm::vec2 Size;      // 字形像素尺寸
        glm::vec2 Bearing;   // 相对光标的偏移(left, top,以基线为原点)
        float Advance;       // 画完这个字符光标前进多少(像素)
    };
    unsigned int DecodeUTF8(const char*& text);
    class GlyphAtlas {
        public:
            static GlyphAtlas & Get(){
                static GlyphAtlas Instance;
                return Instance;
            }
            void Build(const char* fontPath, int pixelSize);
            const Glyph & GetGlyph(unsigned int codepoint) const;
            unsigned int GetTextureID() const;
            float GetPixelSize() const { return m_PixelSize; }
        private:
            GlyphAtlas() = default;
            unsigned int m_TextureID = 0;
            std::unordered_map<unsigned int, Glyph> m_Glyphs;
            bool m_Built = false;
            int m_PixelSize = 0;
    };
}