#pragma once
#include "Quack/Core/Layer.hpp"
#include "Quack/Events/Event.hpp"
#include "Quack/Rendering/Shader.hpp"
#include "Quack/Geometry/Object.hpp"
#include "Quack/Geometry/Primitive.hpp"
#include "Quack/Geometry/Model.hpp"
#include "Quack/Camera/RadialCamera.hpp"
#include "Quack/Events/InputEvent.hpp"
#include "Quack/Events/WindowEvent.hpp"
#include "Quack/Lighting/PointLight.hpp"
#include "Quack/Rendering/ShaderContainer.hpp"

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
        std::unique_ptr<Core::Primitive> m_primitive;
        
        std::unique_ptr<Core::Shader> m_basic_shader;
        std::unique_ptr<Core::ShaderContainer> m_shader_container;
        std::unique_ptr<Core::RadialCamera> m_camera;

        // Store different types of light sources in different vectors.
        // They can be stored in the same vector by storing pointers to LightSource
        std::vector<Core::PointLight> m_point_lights;

        bool onKeyPressed(Core::KeyPressedEvent& event);
        bool onKeyReleased(Core::KeyReleasedEvent& event);
        bool onWindowResize(Core::WindowResizeEvent& event);
        bool onMouseButtonPressed(Core::MouseButtonPressedEvent& event);
        bool onMouseButtonReleased(Core::MouseButtonReleasedEvent& event);
        bool onMouseMoved(Core::MouseMovedEvent& event);
        bool onMouseScrolled(Core::MouseScrolledEvent& event);
};