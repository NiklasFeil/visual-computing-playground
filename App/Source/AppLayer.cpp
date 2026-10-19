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

    m_basic_shader = std::make_unique<Core::Shader>(Core::ShaderConfig("App/Shaders/basic.vert", "App/Shaders/basic.frag"));
    m_basic_shader->compile();

    std::vector<float> vertices = {
        // Positions          // Colors          // UV
        -0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, 0.5f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    };
    std::vector<int> indices = {
        0, 2, 1, 2, 3, 1,
        0, 5, 4, 0, 1, 5,
        0, 6, 2, 0, 4, 6,
        2, 6, 3, 6, 7, 3,
        1, 3, 7, 1, 7, 5,
        7, 6, 4, 5, 7, 4 
    };

    Core::ObjectRenderSpecs o_specs = {
        vertices,
        indices,
        GL_TRIANGLES,
        {3, 3, 2}
    };

    Core::Transform transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    const Core::Model model("App/Models/backpack/backpack.obj");
    m_object = std::make_unique<Core::Object>(model, transform);
    
    m_camera = std::make_unique<Core::RadialCamera>(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);

    Core::Input::SetLastMousePos(Core::Input::GetMousePosX(), Core::Input::GetMousePosY());
}

AppLayer::~AppLayer() {
    std::cout << "Terminating AppLayer" << std::endl;
    m_object.reset();
    m_basic_shader.reset();
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
    //m_object->get_transform().rotate(glm::angleAxis(0.01f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));

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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_basic_shader->use();
    m_basic_shader->set_uniform("uView", m_camera->get_view_matrix());
    m_basic_shader->set_uniform("uProjection", m_camera->get_projection_matrix());
    m_basic_shader->set_uniform("uModel", m_object->get_transform().get_model_matrix());

    m_object->render(*m_basic_shader.get());

    glBindVertexArray(0);
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

    if (Core::Input::IsMouseButtonPressed(Core::MouseButton::Left)) {
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

    std::cout << event.to_string() << std::endl;

    m_camera->add_to_radius(event.get_offset_y());
    m_camera->update_pos();    
    m_camera->update_view();
        
    return false;
}