#include "Lighting/PointLight.hpp"

namespace Core {

    Sphere construct_mesh(const glm::vec3& position, const glm::vec3& color) {        
        Transform transform(position);
        transform.setScale(0.2f);
        Sphere mesh(transform, color, false);
        return mesh;
    }

    PointLight::PointLight(glm::vec3 position, glm::vec3 color, float intensity, float radius, float falloff)
        : LightSource(position, color, intensity), m_radius(radius), m_falloff(falloff), m_mesh(construct_mesh(position, color)) {}

    void PointLight::render(Shader& shader) {
        shader.setUniform("uColor", m_color);
        shader.setUniform("uModel", m_mesh.get_transform().getModelMatrix());
        m_mesh.render(shader);
    }
}