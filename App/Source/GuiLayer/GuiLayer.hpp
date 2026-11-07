#pragma once
#include "Quack/Core/Layer.hpp"
#include "Quack/Gui/Gui.hpp"

class GuiLayer : public Core::Layer {
    public:
        GuiLayer();
        ~GuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onEvent(Core::Event& event) override;
        void onUpdate(float dt) override;
        void onRender() override;

    private:
        bool onGuiInteraction(Core::GuiInteractionEvent& event);
};  