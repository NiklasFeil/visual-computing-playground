#pragma once
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"

namespace Core {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    class Mesh {
        public:
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

            void render(Shader& shader);

        private:

            std::vector<Vertex> m_vertices;
            std::vector<unsigned int> m_indices;
            std::vector<Texture> m_textures;

            // render data
            unsigned int m_vao, m_vbo, m_ebo;

            void setup_mesh();
    };
}