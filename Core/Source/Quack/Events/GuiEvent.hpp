#pragma once

#include "Events/Event.hpp"

namespace Core {

    class GuiButtonPressedEvent : public Event {

        public:

            GuiButtonPressedEvent(std::string button_label)
            : m_button_label(std::move(button_label)) {}

            inline const std::string& getLabel() const {
                return m_button_label;
            }

            std::string toString() const override {
                return "GuiButtonPressedEvent: " + m_button_label;
            }

            EVENT_CLASS_TYPE(GuiButtonPressed);

        private:

            std::string m_button_label;
    };


    class GuiSliderShiftedEvent : public Event {

        public:

            GuiSliderShiftedEvent(std::string label, float value)
            : m_label(std::move(label)), m_value(value) {}

            inline const std::string& getLabel() const {
                return m_label;
            }

            inline const float getValue() const {
                return m_value;
            }

            std::string toString() const override {
                return "GuiSliderShiftedEvent: " + m_label + " has value " + std::to_string(m_value);
            }

            EVENT_CLASS_TYPE(GuiSliderShifted);

        private:

            std::string m_label;
            float m_value;
    };

    class GuiCheckboxChangedEvent : public Event {

        public:

            GuiCheckboxChangedEvent(std::string label, bool value)
            : m_label(std::move(label)), m_value(value) {}

            inline const std::string& getLabel() const {
                return m_label;
            }

            inline const float getValue() const {
                return m_value;
            }

            std::string toString() const override {
                return "GuiCheckboxChangedEvent: " + m_label + " has value " + std::to_string(m_value);
            }

            EVENT_CLASS_TYPE(GuiCheckboxChanged);

        private:

            std::string m_label;
            bool m_value;
    };
}