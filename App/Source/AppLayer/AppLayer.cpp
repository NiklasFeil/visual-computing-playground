#include <vector>
#include <filesystem>
#include <print>
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
#include "Quack/Logger/Logger.hpp"

#include "GuiLayer/GuiLayer.hpp"
#include "AppLayer/AppLayer.hpp"

AppLayer::AppLayer() {}

AppLayer::~AppLayer() {}

void AppLayer::onAttach() {
    Core::Logger::Log("Attach AppLayer", Core::LogLevel::Info);
    m_shader_container = std::make_unique<Core::ShaderContainer>();

    Core::Shader phong_shader("App/Shaders/phong.vert", "App/Shaders/phong.frag");
    phong_shader.compile();
    m_shader_container->add_shader("phong", std::move(phong_shader));

    Core::Shader light_shader("App/Shaders/light.vert", "App/Shaders/light.frag");
    light_shader.compile();
    m_shader_container->add_shader("light", std::move(light_shader));

    Core::Logger::Log("Spawn cow", Core::LogLevel::Info);
    const Core::Model cow("App/Models/cow/cow.obj", true);
    Core::Transform transform_obj(glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    m_object = std::make_unique<Core::Object>(cow, transform_obj);

    Core::Logger::Log("Spawn cube", Core::LogLevel::Info);
    const Core::Model cube(Core::PrimitiveType::Cuboid, glm::vec3{1.0f, 1.0f, 1.0f}, true);
    Core::Transform transform_cube(glm::vec3(0.0f, 0.0f, 0.0f));
    transform_cube.setScale({10.0f, 10.0f, 10.0f});
    m_cube = std::make_unique<Core::Object>(cube, transform_cube);
    
    m_camera = std::make_unique<Core::RadialCamera>(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);

    // Lights
    float initial_intensity = 1.0f, initial_radius = 4.0f, initial_falloff = 5.0f;
    m_point_lights = std::vector<Core::PointLight>();
    m_point_lights.emplace_back(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 4.0f, 5.0f);

    Core::Input::SetLastMousePos(Core::Input::GetMousePosX(), Core::Input::GetMousePosY());

    // Create corresponding GuiWindow
    Core::Logger::Log("Create AppLayer GuiWindow", Core::LogLevel::Info);
    std::shared_ptr<Core::Gui> gui = Core::Application::Get().getGui();
    Core::GuiWindow& gui_window = gui->createGuiWindow("AppLayer");

    gui_window.addWidget<Core::SliderFloat>("applayer_pointlight_intensity", "Pointlight Intensity", {0.1f, 3.0f}, initial_intensity);
    gui_window.addWidget<Core::SliderFloat>("applayer_pointlight_radius", "Pointlight Radius", {0.1f, 20.0f}, initial_radius);
    gui_window.addWidget<Core::SliderFloat>("applayer_pointlight_falloff", "Pointlight Falloff", {0.0f, 15.0f}, initial_falloff);

    gui_window.addWidget<Core::Text>("applayer_lighting_model_text", "Choose lighting model");
    gui_window.addWidget<Core::Button>("applayer_phong_button", "Phong", {50, 50});
    gui_window.addWidget<Core::Button>("applayer_gouraud_button", "Gouraud", {50, 50});
}

void AppLayer::onDetach() {
    Core::Logger::Log("Detach AppLayer", Core::LogLevel::Info);
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
    m_object->getTransform().rotate(glm::angleAxis(0.04f, glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f))));

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

    // Render cow model
    Core::Shader& phong_shader = m_shader_container->get_shader("phong");
    phong_shader.use();
    phong_shader.setCameraUniforms(*m_camera.get());

    for (uint16_t i = 0; i < m_point_lights.size(); ++i) {
        phong_shader.setUniform("uPointLight.color", m_point_lights[i].get_color());
        phong_shader.setUniform("uPointLight.position", m_point_lights[i].get_position());
        phong_shader.setUniform("uPointLight.intensity", m_point_lights[i].get_intensity());
        phong_shader.setUniform("uPointLight.radius", m_point_lights[i].get_radius());
        phong_shader.setUniform("uPointLight.falloff", m_point_lights[i].get_falloff());
    }

    m_object->render(phong_shader);

    // Render cube
    phong_shader.use();
    phong_shader.setCameraUniforms(*m_camera.get());

    for (uint16_t i = 0; i < m_point_lights.size(); ++i) {
        phong_shader.setUniform("uPointLight.color", m_point_lights[i].get_color());
        phong_shader.setUniform("uPointLight.position", m_point_lights[i].get_position());
        phong_shader.setUniform("uPointLight.intensity", m_point_lights[i].get_intensity());
        phong_shader.setUniform("uPointLight.radius", m_point_lights[i].get_radius());
        phong_shader.setUniform("uPointLight.falloff", m_point_lights[i].get_falloff());
    }

    m_cube->render(phong_shader);

    // Render lights
    Core::Shader& light_shader = m_shader_container->get_shader("light");
    light_shader.use();
    light_shader.setCameraUniforms(*m_camera.get());
    
    for (uint16_t i = 0; i < m_point_lights.size(); i++) {
        m_point_lights[i].render(light_shader);
    }

    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

bool AppLayer::onKeyPressed(Core::KeyPressedEvent& event) {
    
    if (event.getKeyCode() == Core::Key::R) {
        std::cout << "Reload shaders" << std::endl;
        m_shader_container->reloadShaders();
        return true;
    }
    
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
        const std::string& label = e->getLabel();
        const float value = e->getValue();
        if (label == "applayer_pointlight_intensity") {
            for (auto& pl : m_point_lights) {
                pl.set_intensity(value);
            }
        }
        else if (label == "applayer_pointlight_radius") {
            for (auto& pl : m_point_lights) {
                pl.set_radius(value);
            }
        }
        else if (label == "applayer_pointlight_falloff") {
            for (auto& pl : m_point_lights) {
                pl.set_falloff(value);
            }
        }

    }
    if (auto* e = event.as<Core::Checkbox, bool>()) {
        std::cout << "Checkbox changed" << std::endl;
    }

    return true;
}