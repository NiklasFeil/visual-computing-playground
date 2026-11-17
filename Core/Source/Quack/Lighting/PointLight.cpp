#include "Lighting/PointLight.hpp"
#include "Geometry/Mesh.hpp"

namespace Core {

    Mesh construct_mesh_point(const glm::vec3& color) {        
        
        Core::MaterialSimple mat{color, color, color, 1.0f};

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::tie(vertices, indices) = Mesh::Cuboid(false);

        Mesh cuboid{vertices, indices, mat};

        return cuboid;
    }

    PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 intensity, float radius, float falloff)
        : LightSource(position, color, intensity), m_radius(radius), m_falloff(falloff), m_mesh(construct_mesh_point(color)) {}

    PointLight::PointLight(glm::vec3 position, glm::vec3 color, float intensity, float radius, float falloff)
        : LightSource(position, color, intensity), m_radius(radius), m_falloff(falloff), m_mesh(construct_mesh_point(color)) {}

    void PointLight::render(Camera& camera, Shader& shader) {
        Transform transform(m_position);
        transform.setScale(0.2f);
        
        shader.setUniform("uColor", m_color);
        shader.setUniform("uModel", transform.getModelMatrix());
        
        m_mesh.render(camera, shader);
    }
}