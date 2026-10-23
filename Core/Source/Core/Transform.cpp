#include "Transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#include <iostream>

namespace Core {

    Transform::Transform()
    : m_translation(glm::vec3(0.0f, 0.0f, 0.0f)), m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_model_up_to_date(false) {}

    Transform::Transform(const glm::vec3& pos)
    : m_translation(pos), m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_model_up_to_date(false) {}

    Transform::Transform(const glm::vec3& pos, const glm::quat& orientation)
    : m_translation(pos), m_orientation(orientation), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_model_up_to_date(false) {}

    Transform::Transform(const glm::vec3& pos, const glm::quat& orientation, const glm::vec3& scale)
    : m_translation(pos), m_orientation(orientation), m_scale(scale), m_model_up_to_date(false) {}


    const glm::vec3 Transform::get_position() const {
        return m_translation;
    };

    const glm::quat& Transform::get_orientation_quat() const {
        return m_orientation;
    };

    const glm::vec3 Transform::get_orientation_euler() const {
        return glm::eulerAngles(m_orientation);
    };

    const glm::mat4& Transform::get_model_matrix() {

        if (!m_model_up_to_date) {
            update_model_matrix();
        }
        return m_model;
    }

    void Transform::update_model_matrix() {
        // Manual calculation. Alternatively, use glm::recompose using skew of (0, 0, 0) and perspective of (0, 0, 0, 1).
        m_model = glm::translate(glm::mat4(1.0f), m_translation) * glm::mat4_cast(m_orientation) * glm::scale(glm::mat4(1.0f), m_scale);
        m_model_up_to_date = true;
    }

    void Transform::set_position(const glm::vec3& new_pos) {
        m_translation = new_pos;
        m_model_up_to_date = false;
    }

    void Transform::set_orientation(const glm::quat& new_orientation) {
        m_orientation = new_orientation;
        m_model_up_to_date = false;
    }

    void Transform::set_scale(const glm::vec3& new_scale) {
        m_scale = new_scale;
        m_model_up_to_date = false;
    }

    void Transform::translate(const glm::vec3& trans) {
        m_translation += trans;
        m_model_up_to_date = false;  
    }

    void Transform::rotate(const glm::quat& rotation) {
        // This is world-space rotation.
        // TODO: Add option for local-space rotation by commuting product.
        m_orientation = glm::normalize(rotation * m_orientation);
        
        m_model_up_to_date = false; 
    }

    // axis must be normalized
    void Transform::rotate(const glm::vec3& axis, const float angle_deg) {
        // TODO: Add option for local-space rotation by commuting product.
        m_orientation += glm::normalize(glm::angleAxis(glm::radians(angle_deg), axis) * m_orientation);
        m_model_up_to_date = false;
    }

    void Transform::scale(const glm::vec3& scale) {
        m_scale *= scale;
        m_model_up_to_date = false;
    }

    void Transform::scale(const float& scale) {
        m_scale *= scale;
        m_model_up_to_date = false;
    }

    Transform Transform::from_model_matrix(const glm::mat4& model) {
        glm::vec3 pos, scale, skew;
        glm::quat orientation;
        glm::vec4 perspective;

        if(!glm::decompose(model, scale, orientation, pos, skew, perspective)) {
            std::cerr << "Decomposing model matrix failed!" << std::endl;
        }

        return Transform(pos, orientation, scale);
    }
}