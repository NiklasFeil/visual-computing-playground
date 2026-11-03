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

    struct Text { std::string t; };
    struct Button { std::string label; ImVec2 dim; EventCallbackFn event_callback; };
    struct SliderFloat { std::string label; ImVec2 minmax; EventCallbackFn event_callback; float value; };
    struct Checkbox { std::string label; EventCallbackFn event_callback; bool value; };

    using Widget = std::variant<Text, Button, SliderFloat, Checkbox>;

    struct VisitWidget {
        void operator()(Text& text)     { ImGui::Text("%s", text.t.c_str()); }
        void operator()(Button& button) { 
            if (ImGui::Button(button.label.c_str(), button.dim)) { 
                GuiButtonPressedEvent event(button.label);
                if(button.event_callback) button.event_callback(event);
            }
        }
        void operator()(SliderFloat& slider) {
            float old = slider.value;
            ImGui::SliderFloat(slider.label.c_str(), &slider.value, slider.minmax.x, slider.minmax.y);
            if (old != slider.value) {
                GuiSliderShiftedEvent event(slider.label, slider.value);
                if(slider.event_callback) slider.event_callback(event);
            }
        }
        void operator()(Checkbox& checkbox) {
            bool old = checkbox.value;
            ImGui::Checkbox(checkbox.label.c_str(), &checkbox.value);
            if (old != checkbox.value) {
                GuiCheckboxChangedEvent event(checkbox.label, checkbox.value);
                if(checkbox.event_callback) checkbox.event_callback(event);
            }
        }
    };

    void renderWidget(Widget& widget);
}