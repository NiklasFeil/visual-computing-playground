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

            /*
            // Done so in the future, the GuiWindow constructor may take more than its title and this stays flexible.
            // Returns reference to the created GuiWindow. 
            //! Error source: Reference might get invalidated when creating new window using this method.
            template <typename ... Args>
            GuiWindow& createGuiWindow(Args&& ... args) {
                m_gui_windows.emplace_back(std::forward<Args>(args) ...); // Returns reference from C++17 onwards
                return m_gui_windows.back();
            }
            */

            GuiWindow& createGuiWindow(std::string title) {
                GuiWindowSpecs specs;
                specs.title = std::move(title);
                specs.event_callback = [this](Event& event) { raiseEvent(event); };
                auto pair = m_gui_windows.emplace(title, specs);
                auto it = pair.first;
                bool inserted = pair.second;
                assert(inserted); // TODO: Throw proper exception
                return (*it).second;
            }

        private:
            GuiConfig m_gui_config;

            std::unordered_map<std::string, GuiWindow> m_gui_windows; // TODO: Exchange string for enum

            void raiseEvent(Event& event);

    };
}