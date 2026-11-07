#pragma once

#include <string>
#include <variant>
#include "Events/GuiEvent.hpp"
#include <iostream>
#include <functional>

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Core {

    using EventCallbackFn = std::function<void(Event&)>;

    struct Text { std::string text; };
    struct Button { std::string text; ImVec2 dim; EventCallbackFn event_callback; };
    struct SliderFloat { std::string text; ImVec2 minmax; EventCallbackFn event_callback; float value; };
    struct Checkbox { std::string text; EventCallbackFn event_callback; bool value; };

    using WidgetId = int;
    using WidgetData = std::variant<Text, Button, SliderFloat, Checkbox>;

    struct VisitWidget {

        void operator()(WidgetId id, std::string label, Text& text)     { ImGui::Text("%s", text.text.c_str()); }
        void operator()(WidgetId id, std::string label, Button& button) { 
            if (ImGui::Button(button.text.c_str(), button.dim)) { 
                GuiInteractionEventTyped<Button, std::monostate> event(id, std::move(label));
                if(button.event_callback) button.event_callback(event);
            }
        }
        void operator()(WidgetId id, std::string label, SliderFloat& slider) {
            float old = slider.value;
            ImGui::SliderFloat(slider.text.c_str(), &slider.value, slider.minmax.x, slider.minmax.y);
            if (old != slider.value) {
                GuiInteractionEventTyped<SliderFloat, float> event(id, std::move(label), slider.value);
                if(slider.event_callback) slider.event_callback(event);
            }
        }
        void operator()(WidgetId id, std::string label, Checkbox& checkbox) {
            bool old = checkbox.value;
            ImGui::Checkbox(checkbox.text.c_str(), &checkbox.value);
            if (old != checkbox.value) {
                GuiInteractionEventTyped<Checkbox, bool> event(id, std::move(label), checkbox.value);
                if(checkbox.event_callback) checkbox.event_callback(event);
            }
        }
    };

    struct Widget {
        WidgetId id;
        std::string label;
        WidgetData data;

        Widget();
        Widget(const Widget& other);
        Widget& operator=(const Widget& other);

        void render() {
            ImGui::PushID(id);
            //std::visit(VisitWidget(), data); -> Cannot be used like this because we also need to supply the id
            std::visit([this](auto& w) { VisitWidget{}(id, label, w); }, data);
            ImGui::PopID();
        }

        private:
            static inline int id_counter = 0;
    };
}