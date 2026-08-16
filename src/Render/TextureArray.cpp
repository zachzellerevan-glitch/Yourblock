#include "TextureArray.h"

namespace Engine{
    void TextureArray::LoadTexture(const std::string &path, int tileSize){
        m_TileSize = tileSize;

        //collect all png file
        std::vector<std::string> TexFiles;
        for(const auto & entry : std::filesystem::directory_iterator(path)){
            if(entry.is_regular_file() && entry.path().extension() == ".png"){
                TexFiles.push_back(entry.path().string());
            }
        }
        if(TexFiles.empty()){
            throw std::runtime_error("No Texture Files(.png) exist in" + path);
        }

        m_LayerCount = static_cast<int>(TexFiles.size());
        m_CpuLayers.resize(m_LayerCount);
        //Get and check size
        int width,height,channels;
        stbi_uc * firstTex = stbi_load(TexFiles[0].c_str(),&width,&height,&channels,4);
        if(!firstTex){
            throw std::runtime_error("Failed to load:" + TexFiles[0]);
        }
        if(width != m_TileSize || height != m_TileSize){
            stbi_image_free(firstTex);
            throw std::runtime_error("Size error");
        }
        stbi_image_free(firstTex);

        //Create Texture Array(without initilized)
        glGenTextures(1,&m_TextureID);
        glBindTexture(GL_TEXTURE_2D_ARRAY,m_TextureID);
        glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA8,m_TileSize,m_TileSize,m_LayerCount,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);

        //load Textures to Array
        for(int i = 0;i < m_LayerCount;i++){
            stbi_uc * Textures = stbi_load(TexFiles[i].c_str(),&width,&height,&channels,4);
            if(!Textures){
                std::cerr<<"Warning:Failed to load:"<<TexFiles[i]<<std::endl;
                continue;
            }
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,i,m_TileSize,m_TileSize,1,GL_RGBA,GL_UNSIGNED_BYTE,Textures);
            m_CpuLayers[i].assign(Textures, Textures + m_TileSize * m_TileSize * 4);
            stbi_image_free(Textures);

            //Set TextureName per Layer
            std::string Name = std::filesystem::path(TexFiles[i]).stem().string();
            m_NameToLayer[Name] = i;
        }

        //Set sample parameter
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    int TextureArray::GetLayer(const std::string &TextureName) const{
        auto it = m_NameToLayer.find(TextureName);
        if(it == m_NameToLayer.end()){
            std::cerr<<"Texture not found:"<<TextureName<<std::endl;
            throw std::runtime_error("Texture not found:" + TextureName);
        }
        return it->second;
    }

    void TextureArray::Bind(uint32_t slot) const{
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY,m_TextureID);
    }

    TextureArray::~TextureArray(){
        if(m_TextureID)
            glDeleteTextures(1,&m_TextureID);
    }


}