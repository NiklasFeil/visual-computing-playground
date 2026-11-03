#include "Gui/GuiWindow.hpp"
#include "Events/GuiEvent.hpp"
#include "Core/Application.hpp"


#include <string>
#include <memory>

namespace Core {

    GuiWindow::GuiWindow(const GuiWindowSpecs& specs)
    : m_gui_window_specs(specs) {
        addWidget<Button>("Test", {140, 280});
        addWidget<SliderFloat>("Slider Test", {0.0, 10.0});
        addWidget<Checkbox>("Checkbox 1", false);
        addWidget<Checkbox>("Checkbox 2", true);
    }

    void GuiWindow::render() {

        ImGui::Begin(m_gui_window_specs.title.c_str());
        
        for (Widget& w : m_widgets) {
            renderWidget(w);
        }

        ImGui::End();
        
    }

    void GuiWindow::raiseEvent(Event& event) {
        if(m_gui_window_specs.event_callback)
            m_gui_window_specs.event_callback(event);
    }
}