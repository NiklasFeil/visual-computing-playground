#pragma once

#include "glm/glm.hpp"

namespace Core {  
    class Camera {
        public:
            Camera(glm::vec3 pos, glm::vec3 target, float fov);
            ~Camera();

            const glm::mat4& get_view_matrix() const;
            const glm::mat4& get_projection_matrix() const;

            glm::vec3 get_position() const;
            glm::vec3 get_target() const;
            glm::vec3 get_direction() const;

            void set_position(glm::vec3 position);
            void set_target(glm::vec3 target);

            void update_view();
            void update_projection();

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