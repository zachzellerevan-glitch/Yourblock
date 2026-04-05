#pragma once

#include "YBpch.h"

namespace Engine{
    class LayerStack{
        public:
            LayerStack();
            ~LayerStack();
            void PushLayer();
            void PopLayer();
            void PushOverlay();
            void PopOverlay();
        private:
            
    };
}