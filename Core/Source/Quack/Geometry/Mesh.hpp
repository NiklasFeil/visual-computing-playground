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

    struct MaterialSimple {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

    using Color = glm::vec3;

    class Mesh {
        public:
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Color color, float shininess);
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialSimple material);

            void render(const Camera& camera, Shader& shader);

            static std::pair<std::vector<Vertex>, std::vector<unsigned int>> Cuboid(bool inverted);
            //static Mesh Sphere();

        private:

            std::vector<Vertex> m_vertices;
            std::vector<unsigned int> m_indices;
            std::vector<Texture> m_textures;
            
            MaterialSimple m_material; // Simple material that can be handcrafted without textures

            bool m_use_textures; 

            // render data
            unsigned int m_vao, m_vbo, m_ebo;

            void setup_mesh();
    };
}