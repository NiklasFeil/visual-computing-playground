#pragma once
#include "Lighting/LightSource.hpp"
#include "Geometry/Primitive.hpp"

namespace Core {
    class DirectionalLight : public LightSource {
        public:
            DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 intensity);
            DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);

            inline void set_direction(glm::vec3 new_direction) { m_direction = new_direction; }
            inline glm::vec3 get_direction() { return m_direction; }

            void render(Shader& shader);

        private:
            glm::vec3 m_direction;
    };
}