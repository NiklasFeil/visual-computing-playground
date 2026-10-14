#include "AppLayer.hpp"
#include <iostream>
#include <vector>
#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include "Core/Transform.hpp"
#include "Core/Model.hpp"
#include "Core/Application.hpp"
#include <filesystem>
#include "glm/gtx/quaternion.hpp"

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

    Core::Transform transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    const Core::Model model("App/Models/backpack/backpack.obj");
    m_object = std::make_unique<Core::Object>(model, transform);
    
    m_camera = std::make_unique<Core::Camera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

AppLayer::~AppLayer() {
    std::cout << "Terminating AppLayer" << std::endl;
    m_object.reset();
    m_basic_shader.reset();
}

void AppLayer::onUpdate(float dt) {
    m_object->get_transform().rotate(glm::angleAxis(0.01f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))));

    auto& app = Core::Application::get();

    auto size = app.get_framebuffer_size();
    auto width = size.x, height = size.y;

    std::cout << "Width: " << width << ", Height: " << height << std::endl;
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