#pragma once

#include "Core/Window/Window.h"
#include "Core/Event/EventHeader.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Layer/Layer.h"
#include "YBpch.h"

namespace Engine{
    class Application{
        public:
            Application(Application & app) = delete;

            void Run();
            void OnEvent(Event &event);
            void PushLayer(Layer * layer);
            void PushOverlay();
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