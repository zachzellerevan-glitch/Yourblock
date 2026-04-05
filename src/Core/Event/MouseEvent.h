#pragma once

#include "YBpch.h"
#include "Event.h"

namespace Engine{
    class MouseMovedEvent : public Event{
        public:
            MouseMovedEvent(double x,double y) : m_MouseX(x),m_MouseY(y) {} 
            inline float GetMouseX(){return m_MouseX;}
            inline float GetMouseY(){return m_MouseY;}
            std::string ToString()const override{
                std::stringstream ss;
                ss<<"MouseMovedEvent:"<<m_MouseX<<","<<m_MouseY;
                return ss.str();
            }
            EVENT_CLASS_TYPE(MouseMoved);
        private:
            float m_MouseX,m_MouseY;
    };

    class MouseButtonEvent : public Event{
        public:
            inline int GetMouseButton(){return m_MouseButton;}
        protected:
            MouseButtonEvent(int mousebutton) : m_MouseButton(mousebutton){}
            int m_MouseButton;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent{
        public:
            MouseButtonPressedEvent(int mousebutton,int repeatcount) : MouseButtonEvent(mousebutton),m_RepeatCount(repeatcount) {}
            inline int GetRepeatCount(){return m_RepeatCount;}
            std::string ToString()const override{
                std::stringstream ss;
                ss<<"MouseButtonPressedEvent:"<<m_MouseButton;
                return ss.str();
            }
            EVENT_CLASS_TYPE(MouseButtonPressed);
        private:
            int m_RepeatCount;
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent{
        public:
            MouseButtonReleasedEvent(int mousebutton) : MouseButtonEvent(mousebutton) {}
            std::string ToString()const override{
                std::stringstream ss;
                ss<<"MouseButtonReleasedEvent:"<<m_MouseButton;
                return ss.str();
            }
            EVENT_CLASS_TYPE(MouseButtonReleased);
    };

    class MouseScrolledEvent : public Event{
        public:
            MouseScrolledEvent(double xOffset,double yOffset)
                : m_xOffset(xOffset) , m_yOffset(yOffset) {}

            inline float GetXOffset() const {return m_xOffset;}
            inline float GetYOffset() const {return m_yOffset;}
            std::string ToString() const override{
                std::stringstream ss;
                ss<<"MouseScrolledEvent";
                return ss.str();
            }
            EVENT_CLASS_TYPE(MouseScrolled);
    
        private:
            float m_xOffset,m_yOffset;
    };
    
}