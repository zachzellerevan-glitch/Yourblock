#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include "Render/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Engine{
    class UILayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
            virtual bool IsEnabled() override {return m_Enable;}
        private:
            std::unique_ptr<Shader> m_UIShader;
            unsigned int m_VAO = 0, m_VBO = 0;
            bool m_Enable = true;
    };
}