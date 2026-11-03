#pragma once

#include <string>
#include <iostream>
#include <functional>
#include "Events/GuiEvent.hpp"
#include "Gui/Widget.hpp"

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Core {

    struct GuiWindowSpecs {
        std::string title;

        using EventCallbackFn = std::function<void(Event&)>; 
        EventCallbackFn event_callback;
    };

    class GuiWindow {
        public:
            GuiWindow(const GuiWindowSpecs& specs);

            void render();

            void addButton(std::string name, ImVec2 dim) {
                if (ImGui::Button(name.c_str(), dim)) {
                    GuiButtonPressedEvent event(name);
                    raiseEvent(event);
                }
            }

            #include "addWidget.inl"

        private:
            GuiWindowSpecs m_gui_window_specs;
            std::vector<Widget> m_widgets;

            void raiseEvent(Event& event);
    };

}