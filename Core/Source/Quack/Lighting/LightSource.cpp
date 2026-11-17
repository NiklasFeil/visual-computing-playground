#include "Lighting/LightSource.hpp"

namespace Core {

    glm::vec3 single_to_multi_intensity(float intensity) {
        return {0.05f * intensity, intensity, 0.3f * intensity};
    }

    LightSource::LightSource(glm::vec3 position, glm::vec3 color, float intensity)
    : m_position(position), m_color(color), m_intensity{single_to_multi_intensity(intensity)} {}

    LightSource::LightSource(glm::vec3 position, glm::vec3 color, glm::vec3 intensity)
    : m_position(position), m_color(color), m_intensity(intensity) {}

    glm::vec3 LightSource::get_position() { return m_position; }
    glm::vec3 LightSource::get_color() { return m_color; }

    glm::vec3 LightSource::get_intensity() { return m_intensity; }
    float LightSource::get_intensity_ambient() { return m_intensity.x; }
    float LightSource::get_intensity_diffuse() { return m_intensity.y; }
    float LightSource::get_intensity_specular() { return m_intensity.z; }

    void LightSource::set_intensity(glm::vec3 new_intensity) { m_intensity = new_intensity; }
    void LightSource::set_intensity(float new_intensity) { m_intensity = single_to_multi_intensity(new_intensity); }
    void LightSource::set_intensity_ambient(float new_ambient) { m_intensity.x = new_ambient; }
    void LightSource::set_intensity_diffuse(float new_diffuse) { m_intensity.y = new_diffuse; }
    void LightSource::set_intensity_specular(float new_specular) { m_intensity.z = new_specular; }
}