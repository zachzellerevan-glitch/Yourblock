#pragma once

#include "Core/Window/Window.h"
#include "Core/Event/EventHeader.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Layer/Layer.h"
#include "Core/Timer/Timer.h"
#include "Core/Input/Input.h"
#include "YBpch.h"

namespace Engine{
    class Application{
        enum class GameState{MainMenu, PauseMenu, InGame};
        public:
            Application(Application & app) = delete;

            void Run();
            void OnEvent(Event &event);
            void PushLayer(Layer * layer);
            void PushOverlay(Layer * layer);
            void PopLayer();
            void PopOverlay();
            inline Window & GetWindow(){return *m_Window;}

            static Application & GetApp(){
                static Application instance;
                return instance;
            }
        private:
            Application();
            ~Application();
            LayerStack m_LayerStack;
            bool OnWindowClose(WindowClosedEvent &event);
            bool m_Running = true;
            std::unique_ptr<Window> m_Window;
    };
}