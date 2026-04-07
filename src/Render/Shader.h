#pragma once

#include "YBpch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
            void SetUniform(const std::string & name,int value);
            void SetUniform(const std::string & name,float value);
            void SetUniform(const std::string & name,const glm::vec3 &value);
            void SetUniform(const std::string & name,const glm::vec4 &value);
            void SetUniform(const std::string & name,const glm::mat4 &value);

            inline unsigned int GetRenderID() const{return m_RenderID;}
        
        private:
            unsigned int CompileShader(unsigned int ShaderType,const std::string & ShaderSource);
            void LinkProgram(unsigned int VertexShader,unsigned int FragmentShader);
            std::string ReadFile(const std::string & path);

            unsigned int m_RenderID;

    };
}