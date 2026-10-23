#pragma once

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Core {
    class Transform {
        public:

            Transform();
            Transform(const glm::vec3& pos);
            Transform(const glm::vec3& pos, const glm::quat& orientation);
            Transform(const glm::vec3& pos, const glm::quat& orientation, const glm::vec3& scale);

            const glm::vec3 get_position() const;
            const glm::quat& get_orientation_quat() const;
            const glm::vec3 get_orientation_euler() const;
            const glm::vec3& get_scale() const;
            const glm::mat4& get_model_matrix();

            void set_position(const glm::vec3& new_pos);
            void set_orientation(const glm::quat& new_orientation);
            void set_scale(const glm::vec3& new_scale);
            
            void translate(const glm::vec3& trans);
            void rotate(const glm::quat& rotation);
            void rotate(const glm::vec3& axis, const float angle_deg);
            void scale(const glm::vec3& scale);
            void scale(const float& scale);

            static Transform from_model_matrix(const glm::mat4& model);

        private:

            glm::vec3 m_translation;
            glm::quat m_orientation;
            glm::vec3 m_scale;

            glm::mat4 m_model;
            bool m_model_up_to_date;
            void update_model_matrix();

    };
}