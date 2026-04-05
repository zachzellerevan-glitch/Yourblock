#include "LayerStack.h"

namespace Engine{
    void LayerStack::PushLayer(Layer * layer){
        m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex,layer);
        m_LayerInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer * layer){
        m_LayerStack.emplace_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PopLayer(Layer * layer){
        auto it = std::find(m_LayerStack.begin(),m_LayerStack.begin() + m_LayerInsertIndex,layer);
        if(it != m_LayerStack.begin() + m_LayerInsertIndex){
            (*it)->OnDetach();
            m_LayerInsertIndex--;
            m_LayerStack.erase(it);
        }
    }

    void LayerStack::PopOverlay(Layer * layer){
        auto it = std::find(m_LayerStack.begin() + m_LayerInsertIndex,m_LayerStack.end(),layer);
        if(it != m_LayerStack.end()){
            (*it)->OnDetach();
            m_LayerStack.erase(it);
        }
    }

    void LayerStack::OnUpdate(){
        for(Layer * layer : m_LayerStack){
            layer->OnUpdate();
        }
    }

    void LayerStack::OnEvent(Event & event){
        for(auto it = m_LayerStack.rbegin();it != m_LayerStack.rend();++it){
            (*it)->OnEvent(event);
            if(event.IsHandled()) break;
        }
    }

    LayerStack::LayerStack(){}

    LayerStack::~LayerStack(){
        for(Layer * layer : m_LayerStack){
            delete layer;
        }
        m_LayerStack.clear();
    }
}