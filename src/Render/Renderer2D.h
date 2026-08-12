#pragma once
#include "YBpch.h"
#include "Render/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Engine{
    struct UIVertex{
            glm::vec2 Position;
            glm::vec2 UV;
            glm::vec4 Color;
    };

    class Renderer2D{
        public:
            Renderer2D();
            ~Renderer2D();

            void BeginFrame(float screenW, float screenH);
            void DrawQuad(float x, float y, float w, float h,
                        const glm::vec4 & color, const glm::vec2 & uvMin = {0.0f,0.0f},
                        const glm::vec2 & uvMax = {1.0f, 1.0f},
                        unsigned int texture = 0);
            void EndFrame();
        private:
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Shader> m_UIShader;
            unsigned int m_QuadVAO = 0, m_QuadVBO = 0;
            unsigned int m_WhiteTexture = 0;
            glm::mat4 m_Ortho;
    };
}