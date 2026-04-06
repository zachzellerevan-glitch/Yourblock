#include "Window.h"
#include "YBpch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Engine{
    Window::Window(const WindowProps & props) : m_Data(props)  {
        //only for debug
        SetEventCallback([](Event & e){});

        static bool glfwInitialized = false;
        if(!glfwInitialized){
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
            glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
            glfwInitialized = true;
        }
        m_Window = glfwCreateWindow(props.Width,props.Height,props.Title.c_str(),nullptr,nullptr);
        
        glfwMakeContextCurrent(m_Window);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cout<<"Error:glad initialized fail"<<std::endl;
        }
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        glViewport(0, 0, width, height);

     

        SetVsync(true);

        glfwSetWindowUserPointer(m_Window,this); //this: store whole Window class

        //Keyboard callback
        glfwSetKeyCallback(m_Window,[](GLFWwindow * window,int key, int scancode, int action, int mods){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if(!SelfWindow || !SelfWindow->m_EventCallback){
                printf("Error:Window.h::Window()::glfwSetKeyCallback\n");
                return;
            }

            switch(action){
                case GLFW_PRESS:{
                    KeyPressedEvent event(key,0);
                    SelfWindow->m_EventCallback(event); // call eventcallbackfn not store
                    break;
                }
                case GLFW_RELEASE:{
                    KeyReleasedEvent event(key);
                    SelfWindow->m_EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    KeyPressedEvent event(key,1);
                    SelfWindow->m_EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window, double xpos, double ypos){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(xpos,ypos);
            SelfWindow->m_EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window,[](GLFWwindow* window, int button, int action, int mods){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            switch(action){
                case GLFW_PRESS:{
                    MouseButtonPressedEvent event(button,0);
                    SelfWindow->m_EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    MouseButtonReleasedEvent event(button);
                    SelfWindow->m_EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    MouseButtonPressedEvent event(button,1);
                    SelfWindow->m_EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window,[](GLFWwindow* window, double xoffset, double yoffset){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(xoffset,yoffset);
            SelfWindow->m_EventCallback(event);
        });

        glfwSetCharCallback(m_Window,[](GLFWwindow* window, unsigned int codepoint){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(codepoint);
            SelfWindow->m_EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window,[](GLFWwindow * window){
            Window * SelfWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowClosedEvent event;
            SelfWindow->m_EventCallback(event);
        });
    }

    Window::~Window(){
        if(m_Window){
            glfwDestroyWindow(m_Window);
        }
    }

    void Window::SetVsync(bool flag){
        if(flag)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.Vsync = flag;
    }

    void Window::OnUpdate(){
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    bool Window::shouldClose() const{
        return ShouldClose || glfwWindowShouldClose(m_Window);
    }

    void Window::SetShouldClose(bool flag){
        ShouldClose = flag;
        glfwSetWindowShouldClose(m_Window,flag);
    }

    void Window::GetFrameBufferSize(int & width,int & height) const {
        glfwGetFramebufferSize(m_Window,&width,&height);
    }

    void Window::HideCursor(bool flag){
        if(flag)
            glfwSetInputMode(m_Window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        else 
            glfwSetInputMode(m_Window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }

    std::unique_ptr<Window> Window::Create(const WindowProps & prop){
        return std::make_unique<Window>(prop);
    }
}