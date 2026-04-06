#include "Shader.h"

namespace Engine{
    Shader::Shader(const std::string & VertexShaderPath,const std::string & FragmentShaderPath){
        std::string VertexCode,FragmentCode;
        VertexCode = ReadFile(VertexShaderPath);
        FragmentCode = ReadFile(FragmentShaderPath);
        const char * C_VertexCode = VertexCode.c_str();
        const char * C_FragmentCode = FragmentCode.c_str();
        unsigned int VertexShader = CompileShader(GL_VERTEX_SHADER,C_VertexCode);
        unsigned int FragmentShader = CompileShader(GL_FRAGMENT_SHADER,C_FragmentCode);
        LinkProgram(VertexShader,FragmentShader);
    }

    Shader::Shader(const std::string & VertexShaderSource,const std::string & FragmentShaderSource,bool FromFile = false){
        const char * C_VertexCode = VertexShaderSource.c_str();
        const char * C_FragmentCode = FragmentShaderSource.c_str();
        unsigned int VertexShader = CompileShader(GL_VERTEX_SHADER,C_VertexCode);
        unsigned int FragmentShader = CompileShader(GL_FRAGMENT_SHADER,C_FragmentCode);
        LinkProgram(VertexShader,FragmentShader);
    }

    unsigned int  Shader::CompileShader(unsigned int ShaderType,const std::string & ShaderSource){
        unsigned int ID = glCreateShader(ShaderType);
        const char * ShaderSrc = ShaderSource.c_str();
        glShaderSource(ID,1,&ShaderSrc,nullptr);
        glCompileShader(ID);

        int CheckCompile;
        char InfoLog[512];
        glGetShaderiv(ID,GL_COMPILE_STATUS,&CheckCompile);
        if(!CheckCompile){
            glGetShaderInfoLog(ID,512,NULL,InfoLog);
            std::string Infolog = InfoLog;
            throw std::runtime_error("Shader Compile error:" + Infolog);
        }

        return ID;
    }

    void Shader::LinkProgram(unsigned int VertexShader,unsigned int FragmentShader){
        unsigned int Program = glCreateProgram();
        glAttachShader(Program,VertexShader);
        glAttachShader(Program,FragmentShader);
        glLinkProgram(Program);
        int CheckCreate;
        char InfoLog[512];
        glGetProgramiv(Program,GL_LINK_STATUS,&CheckCreate);
        if(!CheckCreate){
            glGetProgramInfoLog(Program,512,NULL,InfoLog);
            std::string Infolog = InfoLog;
            throw std::runtime_error("Failed to link program:" + Infolog);
        }

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        m_RenderID = Program;
    }

    std::string Shader::ReadFile(const std::string & path){
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        if (content.empty()) {
            throw std::runtime_error("Shader file is empty: " + path);
        }
        return content;
    }

    void Shader::Use()const {
        glUseProgram(m_RenderID);
    }
}