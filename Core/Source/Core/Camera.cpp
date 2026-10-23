#include "Camera.hpp"
#include "Application.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {

    Camera::Camera(glm::vec3 pos, glm::vec3 target, float fov)
    : m_position(pos), m_target(target), m_fov(fov) {
    
        update_view();

        update_projection();
    }

    Camera::~Camera() {

    }

    const glm::mat4& Camera::get_view_matrix() const {
        return m_view;
    }

    const glm::mat4& Camera::get_projection_matrix() const {
        return m_projection;
    }

    glm::vec3 Camera::get_position() const {
        return m_position;
    }

    void Camera::set_position(glm::vec3 position) {
        m_position = position;
    }

    inline glm::vec3 Camera::get_target() const {
        return m_target;
    }

    void Camera::set_target(glm::vec3 target) {
        m_target = target;
    }

    inline glm::vec3 Camera::get_direction() const {
        return glm::normalize(m_target - m_position);
    }

    /*
    // Local coordinate system (right, dir, up)
    void Camera::shift(glm::vec3 shift) {

    }
    */

    void Camera::update_view() {
        glm::vec3 cameraDirection = glm::normalize(m_position - m_target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        m_view = glm::lookAt(m_position, m_target, cameraUp);
    }

    void Camera::update_projection() {
        glm::vec2 dim = Application::get().get_framebuffer_size();
        m_projection = glm::perspective(glm::radians(m_fov), dim.x / dim.y, 0.1f, 100.0f);
    }
}