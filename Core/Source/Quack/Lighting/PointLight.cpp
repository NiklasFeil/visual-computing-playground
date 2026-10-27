#include "Lighting/PointLight.hpp"

namespace Core {
    PointLight::PointLight(glm::vec3 position, glm::vec3 color, float lux, float radius)
        : LightSource(position, color, lux), m_radius(radius) {}
}