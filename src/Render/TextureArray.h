#pragma once 

#include "YBpch.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>

namespace Engine{
    class TextureArray{
        public:
            static TextureArray & Get(){
                static TextureArray Instance;
                return Instance;
            }

            void LoadTexture(const std::string & path,int tileSize = 16);
            int GetLayer(const std::string & TextureName) const;
            void Bind(uint32_t slot = 0) const;
            inline unsigned int GetTextureID() const {return m_TextureID;}

        private:
            TextureArray() = default;
            ~TextureArray();
            unsigned int m_TextureID = 0;
            int m_TileSize = 0;
            int m_LayerCount = 0;
            std::unordered_map<std::string,int> m_NameToLayer;

    };
}