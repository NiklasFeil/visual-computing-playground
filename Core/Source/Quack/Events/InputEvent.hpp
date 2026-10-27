#pragma once

#include "Events/Event.hpp"

namespace Core {

    // Key Events

    class KeyEvent : public Event {

        public:

            inline int getKeyCode() const {
                return m_key_code;
            }

        protected:

            KeyEvent(int key_code)
            : m_key_code(key_code) {}

            int m_key_code;

    };

    class KeyPressedEvent : public KeyEvent {
        public:
            KeyPressedEvent(int keycode, bool is_repeat)
            : KeyEvent(keycode), m_is_repeat(is_repeat) {}

            inline bool getIsRepeat() const { return m_is_repeat; }

            std::string toString() const override {
                return "KeyPressedEvent: " + std::to_string(m_key_code) + " (repeat=" + std::to_string(m_is_repeat) + ")";
            }

            EVENT_CLASS_TYPE(KeyPressed);

        private:
            bool m_is_repeat;
    };

    class KeyReleasedEvent : public KeyEvent {
        public:
            KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) {}

            std::string toString() const override {
                return "KeyReleasedEvent: " + std::to_string(m_key_code);
            }

            EVENT_CLASS_TYPE(KeyReleased);
    };

    // Mouse Events

    class MouseMovedEvent : public Event {
        public:
            MouseMovedEvent(double x, double y)
            : m_mouse_x(x), m_mouse_y(y) {}

            inline double get_x() const { return m_mouse_x; }
            inline double get_y() const { return m_mouse_y; }

            std::string toString() const override {
                return "MouseMovedEvent: (" + std::to_string(m_mouse_x) + ", " + std::to_string(m_mouse_y) + ")";
            }

            EVENT_CLASS_TYPE(MouseMoved);

        private:
            double m_mouse_x, m_mouse_y;

    };

    class MouseScrolledEvent : public Event {
        public:
            MouseScrolledEvent(double x, double y)
            : m_offset_x(x), m_offset_y(y) {}

            inline double getOffsetX() const { return m_offset_x; }
            inline double getOffsetY() const { return m_offset_y; }

            std::string toString() const override {
                return "MouseScrolledEvent: (" + std::to_string(m_offset_x) + ", " + std::to_string(m_offset_y) + ")";
            }

            EVENT_CLASS_TYPE(MouseScrolled);

        private:
            double m_offset_x, m_offset_y;

    };

    class MouseButtonEvent : public Event {
        public:

            inline int getMouseButton() const {
                return m_button;
            }

        protected:

            MouseButtonEvent(int button)
            : m_button(button) {}

            int m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
        public:
            MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) {}

            std::string toString() const override {
                return "MouseButtonPressedEvent: " + std::to_string(m_button);
            }

            EVENT_CLASS_TYPE(MouseButtonPressed);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
        public:
            MouseButtonReleasedEvent(int keycode)
            : MouseButtonEvent(keycode) {}

            std::string toString() const override {
                return "MouseButtonReleasedEvent: " + std::to_string(m_button);
            }

            EVENT_CLASS_TYPE(MouseButtonReleased);
    };

}