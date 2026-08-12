#pragma once

#include "YBpch.h"
#include "Core/Event/Event.h"
#include "Core/Timer/Timer.h"

namespace Engine{
    class Layer{
        public:
            Layer(const std::string &LayerName = "DebugLayer"){}
            virtual ~Layer(){}
            
            virtual void OnAttach(){}
            virtual void OnDetach(){}
            virtual void OnUpdate(float dt){}
            virtual void OnEvent(Event & event){}
            virtual bool IsEnabled(){return m_Enable;}
        private: 
            bool m_Enable = true;
    };
}