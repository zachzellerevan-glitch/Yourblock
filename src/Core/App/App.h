#pragma once

#include "Event/EventHeader.h"

namespace Engine{
    class Application{
        public:
            Application();
            virtual ~Application();
            void Run();
            void OnEvent(Event &event);
            void PushLayer();
            void PushOverlay();
            void PopLayer();
            void PopOverlay();

            inline static Application & GetApp(){return *s_Instance;}
        private:
            static Application * s_Instance;
    };
}