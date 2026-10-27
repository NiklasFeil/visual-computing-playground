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

            const glm::vec3 getPosition() const;
            const glm::quat& getOrientationQuat() const;
            const glm::vec3 getOrientationEuler() const;
            const glm::vec3& getScale() const;
            const glm::mat4& getModelMatrix();

            void setPosition(const glm::vec3& new_pos);
            void setOrientation(const glm::quat& new_orientation);
            void setScale(const glm::vec3& new_scale);
            
            void translate(const glm::vec3& trans);
            void rotate(const glm::quat& rotation);
            void rotate(const glm::vec3& axis, const float angle_deg);
            void scale(const glm::vec3& scale);
            void scale(const float& scale);

            static Transform FromModelMatrix(const glm::mat4& model);

        private:

            glm::vec3 m_translation;
            glm::quat m_orientation;
            glm::vec3 m_scale;

            glm::mat4 m_model;
            bool m_model_up_to_date;
            void updateModelMatrix();

    };
}