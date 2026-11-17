#pragma once
#include "Lighting/LightSource.hpp"
#include "Geometry/Mesh.hpp"

namespace Core {
    class Spotlight : public LightSource {
        public:
            Spotlight(glm::vec3 position, glm::vec3 color, glm::vec3 intentity, glm::vec3 direction, float radius, float falloff, float inner_cutoff_angle, float outer_cutoff_angle);
            Spotlight(glm::vec3 position, glm::vec3 color, float     intentity, glm::vec3 direction, float radius, float falloff, float inner_cutoff_angle, float outer_cutoff_angle);

            inline void set_direction(glm::vec3 new_direction) { m_direction = new_direction; }
            inline void set_outer_cutoff_angle(float new_angle) { m_outer_cutoff_angle = new_angle; }
            inline void set_inner_cutoff_angle(float new_angle) { m_inner_cutoff_angle = new_angle; }

            inline void set_radius(float new_radius)    { m_radius = new_radius; }
            inline void set_falloff(float new_falloff ) { m_falloff = new_falloff; }

            inline glm::vec3 get_direction() { return m_direction; }

            inline float get_radius() { return m_radius; }
            inline float get_falloff() { return m_falloff; }

            inline float get_outer_cutoff() { return glm::cos(glm::radians(m_outer_cutoff_angle)); }
            inline float get_outer_cutoff_angle() { return m_outer_cutoff_angle; }

            inline float get_inner_cutoff() { return glm::cos(glm::radians(m_inner_cutoff_angle)); }
            inline float get_inner_cutoff_angle() { return m_inner_cutoff_angle; }

            void render(const Camera& camera, Shader& shader);

        private:
            Mesh m_mesh;
            glm::vec3 m_direction;
            float m_radius;
            float m_falloff;
            float m_outer_cutoff_angle; // deg
            float m_inner_cutoff_angle; // deg
    };
}