#include "Lighting/DirectionalLight.hpp"

namespace Core {

    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 intensity)
        : LightSource({0, 0, 0}, color, intensity), m_direction(direction) {}

    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
        : LightSource({0, 0, 0}, color, intensity), m_direction(direction) {}

    void DirectionalLight::render(Shader& shader) {
        // Nothing to render for DirectionalLight
    }
}