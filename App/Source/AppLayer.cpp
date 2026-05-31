#include <GLAD/glad.h>
#include "AppLayer.hpp"
#include <iostream>
#include <vector>
#include "Core/Window.hpp"
#include "Core/Shader.hpp"

AppLayer::AppLayer() {

    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

    m_basic_shader = std::make_unique<Core::Shader>(Core::ShaderConfig(vertexShaderSource, fragmentShaderSource));
    m_basic_shader->compile();

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f
    };
    std::vector<int> indices = {
        0, 1, 2, 1, 2, 3
    };

    Core::ObjectRenderSpecs o_specs = {
        vertices,
        indices,
        GL_TRIANGLES,
        {}
    };

    Core::Transform trans = {};

    m_triangle = std::make_unique<Core::Object>(o_specs, trans);
}

AppLayer::~AppLayer() {
    m_triangle.release();
    m_basic_shader.release();
}

void AppLayer::onUpdate(float dt) {
}

void AppLayer::onRender() {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_basic_shader->get_id());
    m_triangle->render();
    glBindVertexArray(0);
}