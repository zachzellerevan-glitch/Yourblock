#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include "Render/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Engine{
    struct UIVertex{
            glm::vec2 Position;
            glm::vec2 UV;
            glm::vec4 Color;
    };
    class UILayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
            virtual bool IsEnabled() override {return m_Enable;}
        private:
            void DrawQuad(float x, float y, float w, float h,
                        const glm::vec4 & color, const glm::vec2 & uvMin = {0.0f,0.0f},
                        const glm::vec2 & uvMax = {1.0f, 1.0f},
                        unsigned int texture = 0);
        private:
            std::unique_ptr<Shader> m_UIShader;
            unsigned int m_QuadVAO = 0, m_QuadVBO = 0;
            unsigned int m_WhiteTexture = 0;
            bool m_Enable = true;
    };
}