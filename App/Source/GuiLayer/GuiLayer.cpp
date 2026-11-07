#include "GuiLayer/GuiLayer.hpp"

#include "Quack/Core/Application.hpp"
#include "Quack/Gui/Gui.hpp"
#include "Quack/Gui/Widget.hpp"

GuiLayer::GuiLayer() {}

GuiLayer::~GuiLayer() {}

void GuiLayer::onAttach() {

    // Create corresponding GuiWindow
    std::shared_ptr<Core::Gui> gui = Core::Application::Get().getGui();
    Core::GuiWindow& gui_window = gui->createGuiWindow("General");
    gui_window.addWidget<Core::Button>("general_button_showcase", "Switch to Showcase", {500, 75});
    gui_window.addWidget<Core::Button>("general_button_heightmap", "Switch to Heightmap", {500, 75});

}

void GuiLayer::onDetach() {
}

void GuiLayer::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    
    dispatcher.dispatch<Core::GuiInteractionEvent>([this](Core::GuiInteractionEvent& e) { return onGuiInteraction(e); });
}

void GuiLayer::onUpdate(float dt) {
}

void GuiLayer::onRender() {
    
    auto const gui = Core::Application::Get().getGui();
    gui->render();
    
}

bool GuiLayer::onGuiInteraction(Core::GuiInteractionEvent& event) {
    
    /*
    
    */

    return false;
}