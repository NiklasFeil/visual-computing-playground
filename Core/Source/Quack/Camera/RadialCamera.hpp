#pragma once

#include "Camera/Camera.hpp"

namespace Core {

    class RadialCamera : public Camera {
        public:
            RadialCamera(glm::vec3 pos, glm::vec3 target, float fov);
            RadialCamera(float radius, float polar, float azimuth, glm::vec3 target, float fov);

            void moveAlongSphere(glm::vec2 shift, glm::vec2 multiplier);
            void planarMove(glm::vec2 shift);

            void updatePos();

            void addToPolar(float multiplier);
            void addToAzimuth(float multiplier);
            void addToRadius(float multiplier);

            inline float getRadius() const { return m_radius; }
            inline float getPolar() const { return m_polar; }
            inline float getAzimuth() const { return m_azimuth; }

        private:

            static glm::vec3 CalculateSurfacePointFromSphericalCoordinates(glm::vec3 center, float radius, float polar, float azimuth) {
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