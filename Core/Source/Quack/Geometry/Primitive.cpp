#include "Geometry/Primitive.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Renderer.hpp"
#include "GLAD/glad.h"

namespace Core {
    
    void Cuboid::generate_vertices() {

        m_vertices.clear();
        m_indices.clear();
        m_vertices.reserve(24);
        m_indices.reserve(36);

        struct Face {
            glm::vec3 normal;
            glm::vec3 corners[4]; // ccw
        };

        const float h = 0.5f;

        const Face faces[6] = {
            {{ 1, 0, 0}, {{ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}}},  // +X
            {{-1, 0, 0}, {{-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}}},  // -X
            {{ 0, 1, 0}, {{-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}}},  // +Y
            {{ 0,-1, 0}, {{-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}}},  // -Y
            {{ 0, 0, 1}, {{-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}}},  // +Z
            {{ 0, 0,-1}, {{ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}}},  // -Z
        };

        for (const Face& face : faces) {
            const unsigned int base = static_cast<unsigned int>(m_vertices.size());

            for (const auto& corner : face.corners) {
                m_vertices.emplace_back(corner, face.normal);
            }
            m_indices.insert(m_indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
        }
    }

    void Sphere::generate_vertices() {

    }

    void Cylinder::generate_vertices() {

    }

    void Cone::generate_vertices() {

    }
    
    void Primitive::construct_model() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);
    
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(PrimitiveVertex), m_vertices.data(), GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                    &m_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, normal));
    }

    void Primitive::render(Shader& shader) {

        shader.use();

        shader.setUniform("uObjectColor", m_color);

        Renderer::Draw(m_vao, m_indices.size());
        
    }
}