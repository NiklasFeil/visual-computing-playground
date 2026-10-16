#include "Camera.hpp"
#include "Application.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {

    Camera::Camera(glm::vec3 pos, glm::vec3 target, float fov)
    : m_pos(pos), m_target(target), m_fov(fov) {
    
        glm::vec3 cameraDirection = glm::normalize(pos - target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        m_view = glm::lookAt(pos, target, cameraUp);

        update_projection();
    }

    Camera::~Camera() {

    }

    glm::mat4& Camera::get_view_matrix() {
        return m_view;
    }

    glm::mat4& Camera::get_projection_matrix() {
        return m_projection;
    }

    void Camera::update_projection() {
        glm::vec2 dim = Application::get().get_framebuffer_size();
        m_projection = glm::perspective(glm::radians(m_fov), dim.x / dim.y, 0.1f, 100.0f);
    }
}