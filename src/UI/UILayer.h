#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include "Render/Renderer2D.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Game/HotBarState.h"
#include "Game/GameState.h"

namespace Engine{
    
    class UILayer : public Layer{
        public:
            UILayer(const HotbarState* hotbar, const GameUIState* gamestate) 
                : m_Hotbar(hotbar), m_GameUIState(gamestate){}
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
            virtual bool IsEnabled() override {return m_Enable;}
            void DrawHotBar();
        private:
            
        private:
            std::unique_ptr<Renderer2D> m_Renderer;
            bool m_Enable = true;
            bool m_HotbarState = true;
            float m_Width;
            float m_Height;
            float m_FPS = 0.0f;
            const HotbarState* m_Hotbar;
            const GameUIState* m_GameUIState;
    };
}