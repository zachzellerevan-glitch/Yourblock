#pragma once

#include "YBpch.h"
#include "Event.h"

namespace Engine{
    class WindowResizeEvent : public Event{
        public:
        
        private:

    };

    class WindowMovedEvent : public Event{
        public:
        
    };

    class WindowClosedEvent : public Event{
        public:
            WindowClosedEvent(){}
            EVENT_CLASS_TYPE(WindowClose);
    };

    class WindowFocusEvent : public Event{
        public:
            WindowFocusEvent(bool IsFocused) : m_IsFocused(IsFocused){}
            inline bool IsFocused(){return m_IsFocused;}
            std::string ToString() const override{
                std::stringstream ss;
                ss<<"WindowFocusEvent:"<<m_IsFocused;
                return ss.str();
            }
            EVENT_CLASS_TYPE(WindowFocus);
        private:
            bool m_IsFocused;
    };
}