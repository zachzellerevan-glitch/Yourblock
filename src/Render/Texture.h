#pragma once 

#include "YBpch.h"
#include <glad/glad.h>


#include <stb_image/stb_image.h>

namespace Engine{
    class Texture{
        public:
            Texture(const std::string &filepath);
            ~Texture();

            Texture(const Texture&) = delete;
            Texture& operator=(const Texture&) = delete;
            //Texture(Texture&& other) noexcept;
            //Texture& operator=(Texture&& other) noexcept;

            void Bind(uint32_t slot = 0) const;
            void UnBind() const;

            inline int GetWidth(){return m_Width;}
            inline int GetHeight(){return m_Height;}

            void SetFilterMode(GLenum minFilter,GLenum magFilter);
            void SetWrapMode(GLenum wrapS,GLenum wrapT);
        private:
            unsigned int m_RenderID;
            int m_Width,m_Height,m_Channels;
    };
}