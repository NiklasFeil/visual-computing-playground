#include "Geometry/Primitive.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Renderer.hpp"
#include "GLAD/glad.h"
#include <print>

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
        std::array<Face, 6> faces = [&]() -> std::array<Face, 6> {
            if (m_inverted) {
                return {{
                    {{-1, 0, 0}, {{ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}}},  // +X
                    {{ 1, 0, 0}, {{-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}}},  // -X
                    {{ 0,-1, 0}, {{-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}}},  // +Y
                    {{ 0, 1, 0}, {{-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}}},  // -Y
                    {{ 0, 0,-1}, {{-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}}},  // +Z
                    {{ 0, 0, 1}, {{ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}}},  // -Z
                }};
            }
            else {
                return {{
                    {{ 1, 0, 0}, {{ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}}},  // +X
                    {{-1, 0, 0}, {{-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}}},  // -X
                    {{ 0, 1, 0}, {{-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}}},  // +Y
                    {{ 0,-1, 0}, {{-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}}},  // -Y
                    {{ 0, 0, 1}, {{-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}}},  // +Z
                    {{ 0, 0,-1}, {{ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}}},  // -Z
                }};
            }
        }();        

        for (const Face& face : faces) {
            const unsigned int base = static_cast<unsigned int>(m_vertices.size());

            for (const auto& corner : face.corners) {
                m_vertices.emplace_back(corner, face.normal);
            }
            if (!m_inverted) m_indices.insert(m_indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
            else             m_indices.insert(m_indices.end(), {base, base + 2, base + 1, base, base + 3, base + 2});
        }
    }

    void Sphere::generate_vertices() {
        unsigned int sectors = 36, stacks = 18;
        float radius = 0.5f;

        m_vertices.clear();
        m_indices.clear();
        m_vertices.reserve((stacks + 1) * (sectors + 1));
        m_indices.reserve(stacks * sectors * 6);

        const float pi = glm::pi<float>();
        const float s  = m_inverted ? -1.0f : 1.0f;      // flip normals for an inside-out sphere

        // Vertices: (stacks + 1) rings from top pole to bottom pole, (sectors + 1) points per ring
        for (unsigned int i = 0; i <= stacks; ++i) {
            const float theta = pi * static_cast<float>(i) / static_cast<float>(stacks);   // 0 = +Y pole, pi = -Y pole
            const float sin_t = std::sin(theta);
            const float cos_t = std::cos(theta);

            for (unsigned int j = 0; j <= sectors; ++j) {
                const float phi = 2.0f * pi * static_cast<float>(j) / static_cast<float>(sectors);

                const glm::vec3 n(sin_t * std::cos(phi), cos_t, sin_t * std::sin(phi));   // already unit length
                m_vertices.push_back({n * radius, n * s});
            }
        }

        // Indices: two triangles per grid cell
        const unsigned int stride = sectors + 1;
        for (unsigned int i = 0; i < stacks; ++i) {
            for (unsigned int j = 0; j < sectors; ++j) {
                const unsigned int a = i * stride + j;   // top-left
                const unsigned int b = a + stride;       // bottom-left

                // skip the degenerate triangle at each pole (two of its corners coincide)
                if (i != 0) {
                    if (!m_inverted) m_indices.insert(m_indices.end(), {a, a + 1, b});
                    else           m_indices.insert(m_indices.end(), {a, b, a + 1});
                }
                if (i != stacks - 1) {
                    if (!m_inverted) m_indices.insert(m_indices.end(), {a + 1, b + 1, b});
                    else           m_indices.insert(m_indices.end(), {a + 1, b, b + 1});
                }
            }
        }
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
        shader.setUniform("uModel", m_transform.getModelMatrix());

        Renderer::Draw(m_vao, m_indices.size());
    }
}