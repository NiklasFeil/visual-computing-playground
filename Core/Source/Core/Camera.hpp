#pragma once

#include "glm/glm.hpp"

namespace Core {  
    class Camera {
        public:
            Camera(glm::vec3 pos, glm::vec3 target);
            ~Camera();

            glm::mat4& get_view_matrix();
            glm::mat4& get_projection_matrix();
        private:
            glm::mat4 m_view;
            glm::mat4 m_projection;

            glm::vec3 m_pos;
            glm::vec3 m_target;

    };
}