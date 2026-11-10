#pragma once
#include "Lighting/LightSource.hpp"
#include "Geometry/Primitive.hpp"

namespace Core {
    class PointLight : public LightSource {
        public:
            PointLight(glm::vec3 position, glm::vec3 color, float intentity, float radius, float falloff);

            inline float get_radius() { return m_radius; }
            inline float get_falloff() { return m_falloff; }

            inline void set_radius(float new_radius)    { m_radius = new_radius; }
            inline void set_falloff(float new_falloff ) { m_falloff = new_falloff; }

            void render(Shader& shader);

        private:
            Sphere m_mesh;
            float m_radius, m_falloff;
    };
}