#include "Gui/GuiWindow.hpp"
#include "Events/GuiEvent.hpp"
#include "Core/Application.hpp"

#include "imgui/imgui.h"

#include <string>
#include <memory>

namespace Core {

    GuiWindow::GuiWindow(const GuiWindowSpecs& specs)
    : m_gui_window_specs(specs) {}

    void GuiWindow::render() {

        ImGui::Begin(m_gui_window_specs.title.c_str());
        
        for (Widget& w : m_widgets) {
            w.render();
        }

        ImGui::End();
        
    }

    void GuiWindow::raiseEvent(Event& event) {
        if(m_gui_window_specs.event_callback)
            m_gui_window_specs.event_callback(event);
    }
}