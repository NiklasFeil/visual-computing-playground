#include "GuiLayer.hpp"
#include "Quack/Core/Application.hpp"
#include "Quack/Gui/Gui.hpp"

GuiLayer::GuiLayer() {
    onAttach();
}

GuiLayer::~GuiLayer() {
    onDetach();
}

void GuiLayer::onAttach() {

    auto const gui = Core::Application::Get().getGui();
    gui->createGuiWindow("General");
}

void GuiLayer::onDetach() {
}

void GuiLayer::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    
    dispatcher.dispatch<Core::GuiButtonPressedEvent>([this](Core::GuiButtonPressedEvent& e) { return onGuiButtonPressed(e); });
    dispatcher.dispatch<Core::GuiSliderShiftedEvent>([this](Core::GuiSliderShiftedEvent& e) { return onGuiSliderShifted(e); });
    dispatcher.dispatch<Core::GuiCheckboxChangedEvent>([this](Core::GuiCheckboxChangedEvent& e) { return onGuiCheckboxChanged(e); });
}

void GuiLayer::onUpdate(float dt) {
}

void GuiLayer::onRender() {
    
    auto const gui = Core::Application::Get().getGui();
    gui->render();
    
}

bool GuiLayer::onGuiButtonPressed(Core::Event& event) {
    std::cout << event.toString() << std::endl;

    return false;
}

bool GuiLayer::onGuiSliderShifted(Core::Event& event) {
    std::cout << event.toString() << std::endl;

    return false;
}

bool GuiLayer::onGuiCheckboxChanged(Core::Event& event) {
    std::cout << event.toString() << std::endl;

    return false;
}