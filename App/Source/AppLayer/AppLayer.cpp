#include <iostream>
#include <vector>
#include <filesystem>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/string_cast.hpp>

#include "Quack/Core/Window.hpp"
#include "Quack/Rendering/Shader.hpp"
#include "Quack/Geometry/Transform.hpp"
#include "Quack/Geometry/Model.hpp"
#include "Quack/Core/Application.hpp"
#include "Quack/Events/InputEvent.hpp"
#include "Quack/Events/WindowEvent.hpp"
#include "Quack/Camera/RadialCamera.hpp"
#include "Quack/Utils/Utils.hpp"
#include "Quack/Events/Input.hpp"
#include "Quack/Mappings/ButtonMappings.hpp"
#include "Quack/Mappings/KeyMappings.hpp"
#include "Quack/Rendering/Renderer.hpp"
#include "GuiLayer/GuiLayer.hpp"
#include "AppLayer/AppLayer.hpp"

AppLayer::AppLayer() {}

AppLayer::~AppLayer() {}

void AppLayer::onAttach() {
    std::cout << "Attach AppLayer" << std::endl;
    m_shader_container = std::make_unique<Core::ShaderContainer>();

    Core::Shader primitive_phong_shader("App/Shaders/phong.vert", "App/Shaders/phong_primitive.frag");
    primitive_phong_shader.compile();
    m_shader_container->add_shader("primitive phong shader", std::move(primitive_phong_shader));

    Core::Shader model_phong_shader("App/Shaders/phong.vert", "App/Shaders/phong.frag");
    model_phong_shader.compile();
    m_shader_container->add_shader("model phong shader", std::move(model_phong_shader));

    Core::Transform transform_obj(glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    Core::Transform transform_cube(glm::vec3(3.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    const Core::Model model("App/Models/cow/cow.obj", true);
    m_object = std::make_unique<Core::Object>(model, transform_obj);
    m_primitive = std::make_unique<Core::Cuboid>(transform_cube, glm::vec3(1.0f, 0.0f, 0.0f));
    
    m_camera = std::make_unique<Core::RadialCamera>(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);

    m_point_lights = std::vector<Core::PointLight>();
    m_point_lights.emplace_back(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, 10.0f);

    Core::Input::SetLastMousePos(Core::Input::GetMousePosX(), Core::Input::GetMousePosY());

    // Create corresponding GuiWindow
    std::shared_ptr<Core::Gui> gui = Core::Application::Get().getGui();
    Core::GuiWindow& gui_window = gui->createGuiWindow("AppLayer");

    gui_window.addWidget<Core::Text>("applayer_lighting_model_text", "Choose lighting model");
    gui_window.addWidget<Core::Button>("applayer_phong_button", "Phong", {50, 50});
    gui_window.addWidget<Core::Button>("applayer_gouraud_button", "Gouraud", {50, 50});
}

void AppLayer::onDetach() {
    std::cout << "Terminating AppLayer" << std::endl;
    m_object.reset();
    m_shader_container->clear();
    m_basic_shader.reset();
    m_point_lights.clear();
    std::shared_ptr<Core::Gui> gui = Core::Application::Get().getGui();
    gui->destroyGuiWindow("AppLayer");
}

void AppLayer::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    
    dispatcher.dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) { return onKeyPressed(e); });
    //dispatcher.dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& e) { return onKeyReleased(e); });

    dispatcher.dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return onMouseMoved(e); });
    dispatcher.dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return onMouseScrolled(e); });

    dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return onMouseButtonPressed(e); });
    dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) { return onMouseButtonReleased(e); });

    dispatcher.dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return onWindowResize(e); });

    dispatcher.dispatch<Core::GuiInteractionEvent>([this](Core::GuiInteractionEvent& e) { return onGuiInteraction(e); });
}

void AppLayer::onUpdate(float dt) {
    m_object->getTransform().rotate(glm::angleAxis(0.01f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));

    constexpr float multiplier = 0.1f;

    if (Core::Input::IsKeyPressed(Core::Key::W)) {
        m_camera->planarMove(glm::vec2(0.0f, 1.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::A)) {
        m_camera->planarMove(glm::vec2(-1.0f, 0.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::S)) {
        m_camera->planarMove(glm::vec2(0.0f, -1.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::D)) {
        m_camera->planarMove(glm::vec2(1.0f, 0.0f) * multiplier);
    }
}

void AppLayer::onRender() {

    glEnable(GL_DEPTH_TEST);
    Core::Renderer::ClearColorBuffer({0.2f, 0.3f, 0.6f});
    Core::Renderer::ClearDepthBuffer();
    Core::Renderer::EnableBackFaceCulling();

    Core::Shader& model_shader = m_shader_container->get_shader("model phong shader");
    model_shader.use();
    model_shader.setCameraUniforms(*m_camera.get());

    for (uint16_t i = 0; i < m_point_lights.size(); ++i) {
        model_shader.setUniform("uLight.color", m_point_lights[i].get_color());
        model_shader.setUniform("uLight.position", m_point_lights[i].get_position());
    }

    model_shader.setUniform("uModel", m_object->getTransform().getModelMatrix());
    m_object->render(model_shader);


    Core::Shader& primitive_shader = m_shader_container->get_shader("primitive phong shader");
    primitive_shader.use();
    primitive_shader.setCameraUniforms(*m_camera.get());

    for (uint16_t i = 0; i < m_point_lights.size(); ++i) {
        primitive_shader.setUniform("uLight.color", m_point_lights[i].get_color());
        primitive_shader.setUniform("uLight.position", m_point_lights[i].get_position());
    }

    primitive_shader.setUniform("uModel", m_primitive->get_transform().getModelMatrix());
    Core::Renderer::EnableFrontFaceCulling();
    m_primitive->render(primitive_shader);

    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

bool AppLayer::onKeyPressed(Core::KeyPressedEvent& event) {

    /*
    if (event.get_key_code() == Core::Key::Space) {
        std::cout << "Switch" << std::endl;
        TransitionTo<GuiLayer>();
        return true;
    }
    */
    
    return false;
}

bool AppLayer::onKeyReleased(Core::KeyReleasedEvent& event) {

    return false;
}

bool AppLayer::onWindowResize(Core::WindowResizeEvent& event) {

    glViewport(0, 0, event.getWidth(), event.getHeight());
    m_camera->updateProjection();

    return false;
}

bool AppLayer::onMouseButtonPressed(Core::MouseButtonPressedEvent& event) {

    return false;
}

bool AppLayer::onMouseButtonReleased(Core::MouseButtonReleasedEvent& event) {

    return false;
}

bool AppLayer::onMouseMoved(Core::MouseMovedEvent& event) {

    if (Core::Input::IsMouseButtonPressed(Core::MouseButton::Right)) {
        // Move Camera around center
        const glm::vec2 last = Core::Input::GetLastMousePos();
        const glm::vec2 curr = Core::Input::GetMousePos();
        const glm::vec2 dim = Core::Application::Get().getFramebufferSize();
        m_camera->moveAlongSphere((curr - last) / dim, {10.0f, 10.0f});

        return true;
    }

    return false;
}

bool AppLayer::onMouseScrolled(Core::MouseScrolledEvent& event) {

    m_camera->addToRadius(-event.getOffsetY());
    m_camera->updatePos();    
    m_camera->updateView();
        
    return false;
}

bool AppLayer::onGuiInteraction(Core::GuiInteractionEvent& event) {
    //std::cout << event.toString() << std::endl;

    if (event.getLabel().rfind("applayer_", 0) != 0 && event.getLabel().rfind("general_", 0) != 0) {
        // Pass through if widget is not dedicated for applayer content
        return false;
    }

    if (auto* e = event.as<Core::Button>()) {
        const std::string& label = e->getLabel();
        if (label == "general_button_showcase") {
            TransitionTo<AppLayer>();
        }
        else if (label == "general_button_heightmap") {
            TransitionTo<AppLayer>();
        }
    }
    if (auto* e = event.as<Core::SliderFloat, float>()) {
        std::cout << "Slider shifted" << std::endl;
    }
    if (auto* e = event.as<Core::Checkbox, bool>()) {
        std::cout << "Checkbox changed" << std::endl;
    }

    return true;
}