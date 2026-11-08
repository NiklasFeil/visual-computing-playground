#pragma once
#include <functional>
#include <utility>

#include <iostream>
#include "Gui/GuiWindow.hpp"
#include "Core/Window.hpp"

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Core {

    struct GuiConfig {
        // TODO: Include some other config data

        using EventCallbackFn = std::function<void(Event&)>; 
        EventCallbackFn event_callback;
    };

    class Gui {
        public:

            Gui(const GuiConfig& gui_config);
            ~Gui();

            void create(Window& win);

            void render();

            GuiWindow& createGuiWindow(std::string title);
            void destroyGuiWindow(const std::string& title);

            GuiWindow& getGuiWindow(const std::string& title);
            

        private:
 
            GuiConfig m_gui_config;

            std::unordered_map<std::string, GuiWindow> m_gui_windows; // TODO: Exchange string for enum

            void raiseEvent(Event& event);

    };
}