#include "Lighting/Spotlight.hpp"

namespace Core {

    Mesh construct_mesh_spot(const glm::vec3& color) {        
        
        Core::MaterialSimple mat{color, color, color, 1.0f};

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::tie(vertices, indices) = Mesh::Cuboid(false);

        Mesh cuboid{vertices, indices, mat};

        return cuboid;
    }

    Spotlight::Spotlight(glm::vec3 position, glm::vec3 color, glm::vec3 intensity, glm::vec3 direction, float radius, float falloff, float inner_cutoff_angle, float outer_cutoff_angle)
        : LightSource(position, color, intensity), m_direction(direction), m_radius(radius), m_falloff(falloff), m_outer_cutoff_angle(outer_cutoff_angle), m_inner_cutoff_angle(inner_cutoff_angle), m_mesh(construct_mesh_spot(color)) {}

    Spotlight::Spotlight(glm::vec3 position, glm::vec3 color, float intensity, glm::vec3 direction, float radius, float falloff, float inner_cutoff_angle, float outer_cutoff_angle)
        : LightSource(position, color, intensity), m_direction(direction), m_radius(radius), m_falloff(falloff), m_outer_cutoff_angle(outer_cutoff_angle), m_inner_cutoff_angle(inner_cutoff_angle), m_mesh(construct_mesh_spot(color)) {}

    void Spotlight::render(const Camera& camera, Shader& shader) {
        shader.setUniform("uColor", m_color);
        Transform transform(m_position);
        transform.setScale(0.2f);
        shader.setUniform("uModel", transform.getModelMatrix());
        m_mesh.render(camera, shader);
    }
}