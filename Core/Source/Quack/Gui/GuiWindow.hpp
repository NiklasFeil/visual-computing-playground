#pragma once

#include <string>
#include <iostream>
#include <functional>
#include "Events/GuiEvent.hpp"
#include "Gui/Widget.hpp"

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

            #include "addWidget.inl"

        private:
            GuiWindowSpecs m_gui_window_specs;
            std::vector<Widget> m_widgets;

            void raiseEvent(Event& event);
    };

}