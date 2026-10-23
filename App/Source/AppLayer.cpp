#include "AppLayer.hpp"
#include <iostream>
#include <vector>
#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include "Core/Transform.hpp"
#include "Core/Model.hpp"
#include "Core/Application.hpp"
#include "Core/InputEvent.hpp"
#include "Core/WindowEvent.hpp"
#include <filesystem>
#include "glm/gtx/quaternion.hpp"
#include "Core/RadialCamera.hpp"
#include "Core/Utils.hpp"
#include "Core/Input.hpp"
#include "Core/ButtonMappings.hpp"
#include "Core/KeyMappings.hpp"
#include <glm/gtx/string_cast.hpp>

AppLayer::AppLayer() {

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
}

AppLayer::~AppLayer() {
    std::cout << "Terminating AppLayer" << std::endl;
    m_object.reset();
    m_shader_container->clear();
    m_basic_shader.reset();
    m_point_lights.clear();
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
}

void AppLayer::onUpdate(float dt) {
    m_object->get_transform().rotate(glm::angleAxis(0.01f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));

    constexpr float multiplier = 0.1f;

    if (Core::Input::IsKeyPressed(Core::Key::W)) {
        m_camera->planar_move(glm::vec2(0.0f, 1.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::A)) {
        m_camera->planar_move(glm::vec2(-1.0f, 0.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::S)) {
        m_camera->planar_move(glm::vec2(0.0f, -1.0f) * multiplier);
    }
    if (Core::Input::IsKeyPressed(Core::Key::D)) {
        m_camera->planar_move(glm::vec2(1.0f, 0.0f) * multiplier);
    }
}

void AppLayer::onRender() {

    glEnable(GL_DEPTH_TEST);    
    glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Core::Shader& model_shader = m_shader_container->get_shader("model phong shader");
    model_shader.use();
    model_shader.set_camera_uniforms(*m_camera.get());

    for (unsigned int i = 0; i < m_point_lights.size(); ++i) {
        model_shader.set_uniform("uLight.color", m_point_lights[i].get_color());
        model_shader.set_uniform("uLight.position", m_point_lights[i].get_position());
    }

    model_shader.set_uniform("uModel", m_object->get_transform().get_model_matrix());
    m_object->render(model_shader);


    Core::Shader& primitive_shader = m_shader_container->get_shader("primitive phong shader");
    primitive_shader.use();
    primitive_shader.set_camera_uniforms(*m_camera.get());

    for (unsigned int i = 0; i < m_point_lights.size(); ++i) {
        primitive_shader.set_uniform("uLight.color", m_point_lights[i].get_color());
        primitive_shader.set_uniform("uLight.position", m_point_lights[i].get_position());
    }

    primitive_shader.set_uniform("uModel", m_primitive->get_transform().get_model_matrix());
    m_primitive->render(primitive_shader);

    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

bool AppLayer::onKeyPressed(Core::KeyPressedEvent& event) {
    
    return false;
}

bool AppLayer::onKeyReleased(Core::KeyReleasedEvent& event) {

    return false;
}

bool AppLayer::onWindowResize(Core::WindowResizeEvent& event) {

    glViewport(0, 0, event.get_width(), event.get_height());
    m_camera->update_projection();

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
        const glm::vec2 dim = Core::Application::get().get_framebuffer_size();
        m_camera->move_along_sphere((curr - last) / dim, {10.0f, 10.0f});

        return true;
    }

    return false;
}

bool AppLayer::onMouseScrolled(Core::MouseScrolledEvent& event) {

    m_camera->add_to_radius(event.get_offset_y());
    m_camera->update_pos();    
    m_camera->update_view();
        
    return false;
}