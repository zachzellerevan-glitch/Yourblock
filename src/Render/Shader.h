#pragma once

#include "YBpch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Engine{
    class Shader{
        public:
            Shader(const std::string & VertexShaderPath,const std::string & FragmentShaderPath);
            Shader(const std::string & VertexShaderSource,const std::string & FragmentShaderSource,bool FromFile);
            ~Shader(){}

            Shader(const Shader&) = delete;
            Shader& operator=(const Shader&) = delete;
            Shader(Shader&& other) noexcept;
            Shader& operator=(Shader&& other) noexcept;

            void Use() const;

            inline unsigned int GetRenderID() const{return m_RenderID;}
        
        private:
            unsigned int CompileShader(unsigned int ShaderType,const std::string & ShaderSource);
            void LinkProgram(unsigned int VertexShader,unsigned int FragmentShader);
            std::string ReadFile(const std::string & path);

            unsigned int m_RenderID;

    };
}