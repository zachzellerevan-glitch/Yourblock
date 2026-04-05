#pragma once

#include "YBpch.h"
#include "Layer.h"
#include "Core/Event/Event.h"

namespace Engine{
    class LayerStack{
        public:
            LayerStack();
            ~LayerStack();
            void PushLayer(Layer * layer);
            void PopLayer(Layer * layer);
            void PushOverlay(Layer * layer);
            void PopOverlay(Layer * layer);
            void OnUpdate();
            void OnEvent(Event & event);
        private:
            std::vector<Layer*> m_LayerStack;
            unsigned int m_LayerInsertIndex = 0; //mark the last layer of normal layer
    };
}