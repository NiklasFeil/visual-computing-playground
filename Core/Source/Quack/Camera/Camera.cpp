#include "Camera/Camera.hpp"
#include "Core/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {

    Camera::Camera(glm::vec3 pos, glm::vec3 target, float fov)
    : m_position(pos), m_target(target), m_fov(fov) {
    
        updateView();

        updateProjection();
    }

    Camera::~Camera() {

    }

    const glm::mat4& Camera::getViewMatrix() const {
        return m_view;
    }

    const glm::mat4& Camera::getProjectionMatrix() const {
        return m_projection;
    }

    glm::vec3 Camera::getPosition() const {
        return m_position;
    }

    void Camera::setPosition(glm::vec3 position) {
        m_position = position;
    }

    inline glm::vec3 Camera::getTarget() const {
        return m_target;
    }

    void Camera::setTarget(glm::vec3 target) {
        m_target = target;
    }

    inline glm::vec3 Camera::getDirection() const {
        return glm::normalize(m_target - m_position);
    }

    /*
    // Local coordinate system (right, dir, up)
    void Camera::shift(glm::vec3 shift) {

    }
    */

    void Camera::updateView() {
        glm::vec3 cameraDirection = glm::normalize(m_position - m_target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        m_view = glm::lookAt(m_position, m_target, cameraUp);
    }

    void Camera::updateProjection() {
        glm::vec2 dim = Application::Get().getFramebufferSize();
        m_projection = glm::perspective(glm::radians(m_fov), dim.x / dim.y, 0.1f, 100.0f);
    }
}