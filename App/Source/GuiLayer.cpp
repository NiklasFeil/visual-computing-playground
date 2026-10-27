#include "GuiLayer.hpp"
#include "Quack/GUI/gui.hpp"

GuiLayer::GuiLayer() {
    Core::GUI::InitGui();
}

GuiLayer::~GuiLayer() {
    Core::GUI::Cleanup();
}

void GuiLayer::onEvent(Core::Event& event) {

}

void GuiLayer::onUpdate(float dt) {

}

void GuiLayer::onRender() {
    Core::GUI::StartFrame();
    Core::GUI::Render();
}