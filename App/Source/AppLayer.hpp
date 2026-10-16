#pragma once
#include "Core/Layer.hpp"
#include "Core/Event.hpp"
#include "Core/Shader.hpp"
#include "Core/Object.hpp"
#include "Core/Model.hpp"
#include "Core/Camera.hpp"
#include "Core/InputEvent.hpp"
#include "Core/WindowEvent.hpp"
#include <vector>
#include <memory>

class AppLayer : public Core::Layer {

    public:
        AppLayer();
        ~AppLayer();

        void onEvent(Core::Event& event) override;
        void onUpdate(float dt) override;
        void onRender() override;

    private:

        std::unique_ptr<Core::Object> m_object;
        std::unique_ptr<Core::Shader> m_basic_shader;
        std::unique_ptr<Core::Camera> m_camera;

        bool onKeyPressed(Core::KeyPressedEvent& event);
        bool onKeyReleased(Core::KeyReleasedEvent& event);
        bool onWindowResize(Core::WindowResizeEvent& event);
        bool onMouseButtonPressed(Core::MouseButtonPressedEvent& event);
        bool onMouseButtonReleased(Core::MouseButtonReleasedEvent& event);
        bool onMouseMoved(Core::MouseMovedEvent& event);
        bool onMouseScrolled(Core::MouseScrolledEvent& event);

};