#include "Lighting/PointLight.hpp"
#include <print>

namespace Core {

    Sphere construct_mesh(const glm::vec3& position, const glm::vec3& color) {        
        Transform transform(position);
        transform.setScale(0.2f);
        Sphere mesh(transform, color, false);
        return mesh;
    }

    PointLight::PointLight(glm::vec3 position, glm::vec3 color, float lux, float radius)
        : LightSource(position, color, lux), m_radius(radius), m_mesh(construct_mesh(position, color)) {}

    void PointLight::render(Shader& shader) {
        shader.setUniform("uColor", m_color);
        shader.setUniform("uModel", m_mesh.get_transform().getModelMatrix());
        m_mesh.render(shader);
    }
}