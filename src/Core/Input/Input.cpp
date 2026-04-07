#include "Input.h"
#include "Core/App/App.h"

namespace Engine{
    Input::Input(){

    }

    Input::~Input(){

    }

    void Input::ResetMouseDelta(){
        m_FirstMouse = true;
        auto [x, y] = GetMousePos(); 
        m_LastX = x;
        m_LastY = y;
    }

    bool Input::IsKeyPressedImpl(int keycode)
    {
        auto Window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetWindowHandle());
        auto state = glfwGetKey(Window,keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressedImpl(int button){
        auto Window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetWindowHandle());
        auto state = glfwGetMouseButton(Window,button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    std::pair<float, float> Input::GetMousePos(){
        auto Window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetWindowHandle());
        double x,y;
        glfwGetCursorPos(Window,&x,&y);
        return std::pair<float, float>((float)x,(float)y);
    }
    
    std::pair<float, float> Input::GetDeltaMousePosImpl(){
        auto [x,y] = GetMousePos();
        if(m_FirstMouse){
            m_LastX = x;
            m_LastY = y;
            m_FirstMouse = false;
            return {0.0f,0.0f};
        }
        float dx = x - m_LastX;
        float dy = m_LastY - y;
        m_LastX = x;
        m_LastY = y;
        return std::pair<float, float>(dx,dy);
    }
}