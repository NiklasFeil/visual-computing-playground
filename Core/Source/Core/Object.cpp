#include "Object.hpp"
#include <GLAD/glad.h>
#include <utility>
#include <iostream>

namespace Core {
    Object::Object(ObjectRenderSpecs& render_specs, Transform& transform) {

        m_render_specs = std::make_unique<ObjectRenderSpecs>(std::move(render_specs));
        m_transform = std::make_unique<Transform>(std::move(transform));

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);    
        
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_render_specs->vertices.size() * sizeof(float), m_render_specs->vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_render_specs->indices.size() * sizeof(int), m_render_specs->indices.data(), GL_STATIC_DRAW);

        // TODO: Generalize this using m_render_specs.elements_per_vertex_attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);  
        glBindVertexArray(0);

    }

    Object::~Object() {

    }

    void Object::render() {
        glBindVertexArray(m_vao);
        std::cout << "m_render_specs->drawing_method:" << m_render_specs->drawing_method << std::endl;
        std::cout << "GL_TRIANGLES:" << GL_TRIANGLES << std::endl;
        glDrawElements(m_render_specs->drawing_method, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}