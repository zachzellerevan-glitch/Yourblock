#include "Input.h"
#include "Core/App/App.h"

namespace Engine{
    Input::Input(){

    }

    Input::~Input(){

    }

    bool Input::IsKeyPressedImpl(int keycode){
        auto Window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetWindowHandle());
        auto state = glfwGetKey(Window,keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressedImpl(int button){
        auto Window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetWindowHandle());
        auto state = glfwGetMouseButton(Window,button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

 
}