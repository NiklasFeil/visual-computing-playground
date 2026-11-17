#pragma once
#include "Quack/Core/Layer.hpp"
#include "Quack/Events/Event.hpp"
#include "Quack/Rendering/Shader.hpp"
#include "Quack/Scene/Entity.hpp"
#include "Quack/Scene/SceneGraph.hpp"
#include "Quack/Geometry/Primitive.hpp"
#include "Quack/Geometry/Model.hpp"
#include "Quack/Camera/RadialCamera.hpp"
#include "Quack/Events/InputEvent.hpp"
#include "Quack/Events/WindowEvent.hpp"
#include "Quack/Events/GuiEvent.hpp"
#include "Quack/Lighting/PointLight.hpp"
#include "Quack/Lighting/DirectionalLight.hpp"
#include "Quack/Lighting/Spotlight.hpp"
#include "Quack/Rendering/ShaderContainer.hpp"

#include <vector>
#include <memory>

class AppLayer : public Core::Layer {

    public:
        AppLayer();
        ~AppLayer();

        void onAttach() override;
        void onDetach() override;
        void onEvent(Core::Event& event) override;
        void onUpdate(float dt) override;
        void onRender() override;

    private:

        Core::SceneGraph m_scene_graph;
        Core::ShaderContainer m_shader_container;
        std::unique_ptr<Core::RadialCamera> m_camera;

        // Store different types of light sources in different vectors.
        // They can be stored in the same vector by storing pointers to LightSource
        std::vector<Core::PointLight> m_point_lights;
        std::vector<Core::Spotlight> m_spotlights;
        std::unique_ptr<Core::DirectionalLight> m_directional_light;

        bool onKeyPressed(Core::KeyPressedEvent& event);
        bool onKeyReleased(Core::KeyReleasedEvent& event);
        bool onWindowResize(Core::WindowResizeEvent& event);
        bool onMouseButtonPressed(Core::MouseButtonPressedEvent& event);
        bool onMouseButtonReleased(Core::MouseButtonReleasedEvent& event);
        bool onMouseMoved(Core::MouseMovedEvent& event);
        bool onMouseScrolled(Core::MouseScrolledEvent& event);
        bool onGuiInteraction(Core::GuiInteractionEvent& event);
};