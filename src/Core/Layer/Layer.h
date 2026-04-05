#pragma once

#include "YBpch.h"
#include "Core/Event/Event.h"

namespace Engine{
    class Layer{
        public:
            Layer(const std::string &LayerName = "DebugLayer"){}
            virtual ~Layer(){}
            
            virtual void OnAttach(){}
            virtual void OnDetach(){}
            virtual void OnUpdate(){}
            virtual void OnEvent(Event & event){}
    };
}