#pragma once

#include "Event.h"
#include "YBpch.h"

namespace Engine{
    class KeyEvent : public Event{
        public:
            inline int GetKeyCode() const {return m_Keycode;}
        protected:
            KeyEvent(int keycode) : m_Keycode(keycode) {}
            int m_Keycode;
    };

    class KeyPressedEvent : public KeyEvent{
        public:
            KeyPressedEvent(int keycode,int repeatCount) : KeyEvent(keycode),m_RepeatCount(repeatCount) {}
            inline int GetRepeatCount(){return m_RepeatCount;}

            std::string ToString() const override{
                std::stringstream ss;
                ss<<"KeyPressedEvent:"<<m_Keycode;
                return ss.str();
            }
            EVENT_CLASS_TYPE(KeyPressed);
        private:
            int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent{
        public:
            KeyReleasedEvent(int keycode): KeyEvent(keycode){}
            std::string ToString()const override{
                std::stringstream ss;
                ss<<"KeyReleasedEvent:"<<m_Keycode;
                return ss.str();
            }
            EVENT_CLASS_TYPE(KeyReleased);
    };

    class KeyTypedEvent : public KeyEvent{
        public:
            KeyTypedEvent(int keycode)
                : KeyEvent(keycode){}
        
            inline int GetRepeatCount() const {return m_repeatCount;}

            std::string ToString()const override{
                std::stringstream ss;
                ss<<"KeyTypeedEvent:"<<m_Keycode;
                return ss.str();
            }

            EVENT_CLASS_TYPE(KeyTyped);
        private:
            int m_repeatCount;
    };
}