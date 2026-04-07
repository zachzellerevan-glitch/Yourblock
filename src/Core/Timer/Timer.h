#pragma once

#include "YBpch.h"

namespace Engine{
    class Timer{
        public:
            static Timer & Get(){
                static Timer instance;
                return instance;
            }

            void Update(){
                auto now = std::chrono::steady_clock::now();
                m_DeltaTime = std::chrono::duration<float>(now - m_LastTime).count();
                m_LastTime = now;
            }

            inline float GetDeltaTime() const {return m_DeltaTime;}
        private:
            Timer() : m_LastTime(std::chrono::steady_clock::now()),m_DeltaTime(0.0f){}
            float m_DeltaTime;
            std::chrono::steady_clock::time_point m_LastTime;
    };
}