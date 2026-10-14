#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {

    Camera::Camera(glm::vec3 pos, glm::vec3 target) {
    
        glm::vec3 cameraDirection = glm::normalize(pos - target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        m_view = glm::lookAt(pos, target, cameraUp);
        m_pos = pos;
        m_target = target;

        // TODO: Get the actual width and height and update on window resize
        m_projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    }

    Camera::~Camera() {

    }

    glm::mat4& Camera::get_view_matrix() {
        return m_view;
    }

    glm::mat4& Camera::get_projection_matrix() {
        return m_projection;
    }
}