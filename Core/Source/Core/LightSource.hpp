#pragma once

#include "glm/vec3.hpp"

namespace Core {
    class LightSource {
        public:
            LightSource(glm::vec3 position, glm::vec3 color, float lux)
            : m_position(position), m_color(color), m_lux(lux) {}

            inline glm::vec3 get_position() { return m_position; }
            inline glm::vec3 get_color() { return m_color; }
            inline float get_lux() { return m_lux; }

        private:
            glm::vec3 m_position;
            glm::vec3 m_color;
            float m_lux;
    };
}