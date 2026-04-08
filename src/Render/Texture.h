#pragma once 

#include "YBpch.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Engine{
    class Texture{
        public:
            Texture(std::string &filepath);
            ~Texture();


    };
}