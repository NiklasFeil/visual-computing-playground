#pragma once

#include <string>
#include <functional>

namespace Core {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

#define EVENT_CLASS_TYPE(type) static EventType get_static_type() { return EventType::type; } \
                                virtual EventType get_event_type() const override { return get_static_type(); } \
                                virtual const char* get_name() const override { return #type; }

    class Event {
        public:

            bool handled = false;

            virtual ~Event() {};
            virtual EventType get_event_type() const = 0;
            virtual const char* get_name() const = 0;
            virtual std::string to_string() const { return get_name(); }
    };

    class EventDispatcher {

        template<typename T>
        using EventFn = std::function<bool(T&)>;

        public:
            EventDispatcher(Event& event)
            : m_event(event) {}

            template<typename T>
            requires(std::is_base_of_v<Event, T>)
            bool dispatch(EventFn<T> func) {
                if (m_event.get_event_type() == T::get_static_type() && !m_event.handled) {
                    m_event.handled = func(*(T*)&m_event);
                    return true;
                }
                return false;
            }

        private:
            Event& m_event;

    };
}