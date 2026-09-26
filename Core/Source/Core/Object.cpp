#include "Object.hpp"
#include <GLAD/glad.h>
#include <utility>
#include <iostream>
#include <numeric>

namespace Core {
    Object::Object(const ObjectRenderSpecs& render_specs, const Transform& transform, const TextureConfig& texture_config)
    : m_texture(texture_config) {

        m_render_specs = render_specs;
        m_transform = transform;

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);    
        
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_render_specs.vertices.size() * sizeof(float), m_render_specs.vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_render_specs.indices.size() * sizeof(int), m_render_specs.indices.data(), GL_STATIC_DRAW);

        unsigned int num_attributes_per_vertex = std::reduce(m_render_specs.elements_per_vertex_attribute.begin(), m_render_specs.elements_per_vertex_attribute.end());
        
        for (size_t i = 0; i < m_render_specs.elements_per_vertex_attribute.size(); ++i) {
            unsigned int offset = std::reduce(m_render_specs.elements_per_vertex_attribute.begin(), m_render_specs.elements_per_vertex_attribute.begin() + i);
            glVertexAttribPointer(i, m_render_specs.elements_per_vertex_attribute[i], GL_FLOAT, GL_FALSE, num_attributes_per_vertex * sizeof(float), (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(i);
        }
        
        glBindVertexArray(0);
    }

    Object::Object(const ObjectRenderSpecs& render_specs, const glm::mat4& model, const TextureConfig& texture_config)
    : m_texture(texture_config) {

        m_render_specs = render_specs;
        m_transform = Transform::from_model_matrix(model);

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);    
        
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_render_specs.vertices.size() * sizeof(float), m_render_specs.vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_render_specs.indices.size() * sizeof(int), m_render_specs.indices.data(), GL_STATIC_DRAW);

        unsigned int num_attributes_per_vertex = std::reduce(m_render_specs.elements_per_vertex_attribute.begin(), m_render_specs.elements_per_vertex_attribute.end());
        
        for (size_t i = 0; i < m_render_specs.elements_per_vertex_attribute.size(); ++i) {
            unsigned int offset = std::reduce(m_render_specs.elements_per_vertex_attribute.begin(), m_render_specs.elements_per_vertex_attribute.begin() + i);
            glVertexAttribPointer(i, m_render_specs.elements_per_vertex_attribute[i], GL_FLOAT, GL_FALSE, num_attributes_per_vertex * sizeof(float), (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(i);
        }
        
        glBindVertexArray(0);
    }

    void Object::render() {
        glEnable(GL_DEPTH_TEST);
        m_texture.bind();
        glBindVertexArray(m_vao);
        glDrawElements(m_render_specs.drawing_method, m_render_specs.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        m_texture.unbind();
        glDisable(GL_DEPTH_TEST);
    }

    Object::~Object() {
        std::cout << "Object destroyed" << std::endl;
    }

    Transform& Object::get_transform() {
        return m_transform;
    }

}