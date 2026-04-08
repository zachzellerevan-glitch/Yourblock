#include "Texture.h"

namespace Engine{
    Texture::Texture(const std::string &filepath){
        unsigned char * TexData = stbi_load(filepath.c_str(),&m_Width,&m_Height,&m_Channels,0);
        if(!TexData){
            throw std::runtime_error("Texture load error:" + filepath);
        }
        glGenTextures(1,&m_RenderID);
        glBindTexture(GL_TEXTURE_2D,m_RenderID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format = (m_Channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D,0,format,m_Width,m_Height,0,format,GL_UNSIGNED_BYTE,TexData);

        stbi_image_free(TexData);
    }

    Texture::~Texture(){
        if(m_RenderID){
            glDeleteTextures(1,&m_RenderID);
        }
    }

    void Texture::Bind(uint32_t slot) const{
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D,m_RenderID);
    }

    void Texture::UnBind() const{
        glBindTexture(GL_TEXTURE_2D,0);
    }
}