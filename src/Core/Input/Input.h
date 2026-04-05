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
        protected:
            bool IsKeyPressedImpl(int keycode);
            bool IsMouseButtonPressedImpl(int button);
        private:
            Input();
            ~Input();
    };
}