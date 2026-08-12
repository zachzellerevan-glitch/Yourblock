#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include "Render/Renderer2D.h"
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
            
        private:
            std::unique_ptr<Renderer2D> m_Renderer;
            bool m_Enable = true;
    };
}