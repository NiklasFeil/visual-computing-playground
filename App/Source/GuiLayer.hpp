#pragma once
#include "Quack/Core/Layer.hpp"

class GuiLayer : public Core::Layer {
    public:
        GuiLayer();
        ~GuiLayer();

        void onEvent(Core::Event& event) override;
        void onUpdate(float dt) override;
        void onRender() override;
};