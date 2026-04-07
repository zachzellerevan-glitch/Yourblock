#pragma once

#include "YBpch.h"
#include <glfw/glfw3.h>

namespace Engine{
    class Input{
        public:
            static Input & Get(){
                static Input Instance;
                return Instance;
            }
            inline static bool IsKeyPressed(int keycode){return Get().IsKeyPressedImpl(keycode);}
            inline static bool IsMouseButtonPressed(int button){return Get().IsMouseButtonPressedImpl(button);}
            inline static float GetMouseX(){return Get().GetMousePos().first;}
            inline static float GetMouseY(){return Get().GetMousePos().second;}
            inline static std::pair<float,float> GetDeltaMousePos(){return Get().GetDeltaMousePosImpl();}
            void ResetMouseDelta();
        protected:
            bool IsKeyPressedImpl(int keycode);
            bool IsMouseButtonPressedImpl(int button);
            std::pair<float,float> GetMousePos();
            std::pair<float,float> GetDeltaMousePosImpl();
        private:
            float m_LastX = 0.0f, m_LastY = 0.0f;
            bool m_FirstMouse = true;
            Input();
            ~Input();
    };
}