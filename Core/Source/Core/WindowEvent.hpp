#pragma once

#include "Event.hpp"

namespace Core {

    class WindowCloseEvent : public Event {
        public:

            WindowCloseEvent() {}

            EVENT_CLASS_TYPE(WindowClose);
    };

    class WindowResizeEvent : public Event {
        public:

            WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width(width), m_height(height) {}

            inline unsigned int get_width() const { return m_width; }
            inline unsigned int get_height() const { return m_height; }

            std::string to_string() const override {
                return "WindowResizeEvent: (" + std::to_string(m_width) + ", " + std::to_string(m_height) + ")";
            }

            EVENT_CLASS_TYPE(WindowResize);

        private:
            unsigned int m_width, m_height;
    };
}