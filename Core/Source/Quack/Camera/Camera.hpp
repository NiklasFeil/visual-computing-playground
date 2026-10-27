#pragma once

#include "glm/glm.hpp"

namespace Core {  
    class Camera {
        public:
            Camera(glm::vec3 pos, glm::vec3 target, float fov);
            ~Camera();

            const glm::mat4& getViewMatrix() const;
            const glm::mat4& getProjectionMatrix() const;

            glm::vec3 getPosition() const;
            glm::vec3 getTarget() const;
            glm::vec3 getDirection() const;

            void setPosition(glm::vec3 position);
            void setTarget(glm::vec3 target);

            void updateView();
            void updateProjection();

        protected:

            glm::mat4 m_view;
            glm::mat4 m_projection;

            glm::vec3 m_position;
            glm::vec3 m_target;
            float m_fov; // deg
    };
    class FreeCamera : public Camera {
        public:

        private:
    };

}