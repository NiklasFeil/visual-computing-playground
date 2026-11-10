#pragma once

#include "glm/vec3.hpp"
#include "Geometry/Transform.hpp"

namespace Core {
    class LightSource {
        public:
            LightSource(glm::vec3 position, glm::vec3 color, float intensity)
            : m_position(position), m_color(color), m_intensity(intensity) {}

            inline glm::vec3 get_position() { return m_position; }
            inline glm::vec3 get_color() { return m_color; }
            inline float get_intensity() { return m_intensity; }
            inline void set_intensity(float new_intensity) { m_intensity = new_intensity; }

        protected:
            glm::vec3 m_position;
            glm::vec3 m_color;
            float m_intensity;
    };
}