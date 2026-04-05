#include "YBpch.h"
#include "App.h"
#include "Core/Input/Input.h"
#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

namespace Engine{
    class ExampleLayer : public Layer{
        public:
            virtual void OnUpdate() override{
                if(Input::IsKeyPressed(GLFW_KEY_W))
                    printf("W pressed.\n");
            }
    };
    
    Application::Application(){
        m_Window = Window::Create();
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        PushLayer(new ExampleLayer);
    }

    Application::~Application(){

    }

    void Application::Run(){
        while(m_Running){
            glClearColor(0.5f,0.0f,1.0f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
            m_Window->OnUpdate();
            m_LayerStack.OnUpdate();
        }
    }

    void Application::OnEvent(Event & event){
        EventDispatcher dispathcer(event);
        dispathcer.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        m_LayerStack.OnEvent(event);
    }

    bool Application::OnWindowClose(WindowClosedEvent & event){
        printf("Window closed.\n");
        m_Running = false;
        return true;
    }

    void Application::PushLayer(Layer * layer){
        m_LayerStack.PushLayer(layer);
    }
}