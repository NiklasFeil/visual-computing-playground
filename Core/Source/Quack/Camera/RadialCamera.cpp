#include "Camera/RadialCamera.hpp"
#include <math.h>

namespace Core {
    RadialCamera::RadialCamera(glm::vec3 pos, glm::vec3 target, float fov)
    : Camera(pos, target, fov), m_radius(glm::distance(pos, target)) {
        
        glm::vec3 xyz = m_position - m_target;

        // Calculate azimuth angle (angle to x axis in xz plane)
        m_azimuth = acos(xyz.x / glm::length(glm::vec2(xyz.x, xyz.z))); 

        // Calculate polar angle (angle to y axis)
        m_polar = acos((pos.y - target.y) / m_radius);
    }

    RadialCamera::RadialCamera(float radius, float polar, float azimuth, glm::vec3 target, float fov)
    : Camera(CalculateSurfacePointFromSphericalCoordinates(target, radius, polar, azimuth), target, fov),
      m_radius(radius), m_polar(polar), m_azimuth(azimuth) {}

    void RadialCamera::moveAlongSphere(glm::vec2 shift, glm::vec2 multiplier) {
        addToAzimuth(shift.x * multiplier.x);
        addToPolar(-shift.y * multiplier.y);
        updatePos();
        updateView();
    }

    void RadialCamera::planarMove(glm::vec2 shift) {
        
        glm::vec3 direction = glm::normalize(m_target - m_position);
        glm::vec3 global_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 local_right = glm::cross(direction, global_up);
        glm::vec3 local_up = glm::cross(local_right, direction);

        setTarget(m_target + shift.x * local_right + shift.y * local_up);
        setPosition(m_position + shift.x * local_right + shift.y * local_up);

        updateView();
    }

    void RadialCamera::updatePos() {
        m_position = CalculateSurfacePointFromSphericalCoordinates(m_target, m_radius, m_polar, m_azimuth);
    }
    
    void RadialCamera::addToPolar(float change) {
        m_polar = glm::clamp(m_polar + change, 0.01f, static_cast<float>(M_PI) - 0.01f);
    }

    void RadialCamera::addToAzimuth(float change) {
        m_azimuth += change;
        m_azimuth = std::fmod(m_azimuth, 2.0f * M_PI);
    }

    void RadialCamera::addToRadius(float change) {
        m_radius += change;
        if (m_radius < 0.01f) m_radius = 0.01f;
    }
}