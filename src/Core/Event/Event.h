#pragma once


#include "YBpch.h"

namespace Engine{
    enum class EventType{
        None = 0,
        WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
        AppTick,AppUpdate,AppRender,
        KeyPressed,KeyReleased,KeyTyped,
        MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled  
    };

    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::type;}\
                                    virtual EventType GetEventType() const override{return GetStaticType();}\
                                    virtual const char * GetName() const override{return #type;}

    class Event{
        friend class EventDispatcher;
        public:
            virtual EventType GetEventType() const = 0;
            virtual const char * GetName() const = 0;
            virtual ~Event() = default;
            virtual std::string ToString() const {return GetName();}
            bool IsHandled() const {return m_Handled;}
        private:
            bool m_Handled = false;
    };

    class EventDispatcher{
        public:
            EventDispatcher(Event & event) : m_Event(event) {}

            //if EventType is the same as T,than call a callbackfn,and set m_Handled = true
            template<typename T,typename F>
            bool Dispatch(const F& func){
                if(m_Event.GetEventType() == T::GetStaticType()){
                    m_Event.m_Handled = func(static_cast<T&>(m_Event));
                    return true;
                }
                return false;
            }
        private:
            Event & m_Event;
    };
} 
