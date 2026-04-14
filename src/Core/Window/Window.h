#pragma once

#include "YBpch.h"
#include "Core/Event/EventHeader.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Engine{
    class Window{
        public:
            using EventCallbackFn = std::function<void(Event&)>;
            void SetEventCallback(const EventCallbackFn& callback){m_EventCallback = callback;}
            struct WindowProps{
                std::string Title = "YourBlock";
                unsigned int Width = 1920;
                unsigned int Height = 1080;
                bool Resizable = true;
                bool Vsync = true;

                WindowProps(const std::string & title = "YourBlock",unsigned int width = 1280,unsigned int height = 720,bool resizable = true,bool vsync = true)
                    : Title(title),Width(width),Height(height),Resizable(resizable),Vsync(vsync){}
            };

            explicit Window(const WindowProps & props = WindowProps());
            ~Window();
            
            //prohibit copy,move(Singleton?)
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
            Window(Window&& other) noexcept;
            Window& operator=(Window&& other) noexcept;

           void OnUpdate();

            bool shouldClose() const;

            void SetShouldClose(bool flag);

            void GetFrameBufferSize(int &width,int &height) const;

            inline unsigned int GetWidth() const{return m_Data.Width;}
            inline unsigned int GetHeight() const{return m_Data.Height;}

            void SetVsync(bool flag);

            void HideCursor(bool flag);
            static std::unique_ptr<Window> Create(const WindowProps & prop = WindowProps());

            inline GLFWwindow * GetWindowHandle() const{return m_Window;}
        private:
           
            EventCallbackFn m_EventCallback; //store event callback
            GLFWwindow * m_Window = nullptr;
            bool ShouldClose = false;
            WindowProps m_Data;

    };
}