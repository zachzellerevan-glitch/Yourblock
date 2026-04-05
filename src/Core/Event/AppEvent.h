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
}