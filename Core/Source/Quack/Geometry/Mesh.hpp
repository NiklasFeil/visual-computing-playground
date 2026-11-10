#pragma once
#include <variant>
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

    using Color = glm::vec3;

    class Mesh {
        public:
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Color color);

            void render(Shader& shader);

            static Mesh Cuboid(std::variant<Texture, Color> tex, bool inverted);
            //static Mesh Sphere();

        private:

            std::vector<Vertex> m_vertices;
            std::vector<unsigned int> m_indices;
            std::vector<Texture> m_textures;
            glm::vec3 m_color; // color as alternative to textures

            bool m_use_color;

            // render data
            unsigned int m_vao, m_vbo, m_ebo;

            void setup_mesh();
    };
}