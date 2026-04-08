#pragma once

#include "Core/Core.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "YBpch.h"

namespace Engine{
    class GameLayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
        private:
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Camera> m_Camera;
            std::unique_ptr<Texture> m_Texture;
            bool m_FirstFrame = true;
            unsigned int m_VAO,m_VBO,m_EBO;
    };
}