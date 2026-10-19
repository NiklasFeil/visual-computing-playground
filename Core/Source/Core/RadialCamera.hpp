#pragma once

#include "Camera.hpp"
#include "iostream"

namespace Core {

    class RadialCamera : public Camera {
        public:
            RadialCamera(glm::vec3 pos, glm::vec3 target, float fov);
            RadialCamera(float radius, float polar, float azimuth, glm::vec3 target, float fov);

            void move_along_sphere(glm::vec2 shift, glm::vec2 multiplier);
            void planar_move(glm::vec2 shift);

            void update_pos();

            void add_to_polar(float multiplier);
            void add_to_azimuth(float multiplier);
            void add_to_radius(float multiplier);

            inline float get_radius() const { return m_radius; }
            inline float get_polar() const { return m_polar; }
            inline float get_azimuth() const { return m_azimuth; }

        private:

            static glm::vec3 calculate_surface_point_from_spherical_coordinates(glm::vec3 center, float radius, float polar, float azimuth) {
                float x = cos(azimuth) * sin(polar) * radius;
                float y =                cos(polar) * radius; // In OpenGL, y-axis is up
                float z = sin(azimuth) * sin(polar) * radius;
                
                return center + glm::vec3(x, y, z);
            };

            float m_radius;
            float m_polar; // rad
            float m_azimuth; // rad
    };

}