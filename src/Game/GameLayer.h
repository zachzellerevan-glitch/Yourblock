#pragma once

#include "Core/Core.h"
#include "Render/Shader.h"
#include "YBpch.h"

namespace Engine{
    class GameLayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate() override;
            virtual void OnEvent(Event & event) override{}  
        private:
            std::unique_ptr<Shader> m_Shader;
            unsigned int m_VAO,m_VBO;
    };
}