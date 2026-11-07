#pragma once

#include "Events/Event.hpp"
#include <typeindex>
#include <variant>

namespace Core {

    template <typename wT, typename vT>
    class GuiInteractionEventTyped;
    
    class GuiInteractionEvent : public Event {
        public:

            inline const int getId() const {
                return m_id;
            }

            inline const std::string getLabel() const {
                return m_label;
            }

            std::string toString() const override {
                return "GuiInteractionEvent: " + m_label;
            }

            template <typename wT>
            bool isWidget() const {
                return m_widget_type == std::type_index(typeid(wT));
            }

            template<typename wT, typename vT = std::monostate>
            const GuiInteractionEventTyped<wT, vT>* as() const {
                return dynamic_cast<const GuiInteractionEventTyped<wT, vT>*>(this);
            }

            EVENT_CLASS_TYPE(GuiInteraction);

        protected:
            GuiInteractionEvent(const int id, std::string label, std::type_index widget_type)
            : m_id(id), m_label(std::move(label)), m_widget_type(widget_type) {}

        private:
            const int m_id;
            //const std::string m_gui_window_name;
            const std::string m_label;
            const std::type_index m_widget_type;
    };

    template <typename wT, typename vT = std::monostate>
    class GuiInteractionEventTyped : public GuiInteractionEvent {
        public:
            GuiInteractionEventTyped(const int id, std::string label, vT value = {})
            : GuiInteractionEvent(id, std::move(label), typeid(wT)), m_value(value) {}

            const vT& getValue() const {
                return m_value;
            }

        private:
            const vT m_value;

    };
}