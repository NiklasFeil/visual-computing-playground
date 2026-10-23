#pragma once
#include "LightSource.hpp"

namespace Core {
    class PointLight : public LightSource {
        public:
            PointLight(glm::vec3 position, glm::vec3 color, float lux, float radius);

            inline float get_radius() { return m_radius; }

        private:
            float m_radius;
    };
}