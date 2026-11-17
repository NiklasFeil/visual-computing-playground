#pragma once

#include "Geometry/Transform.hpp"

namespace Core {

    class LightSource {
        public:
            LightSource(glm::vec3 position, glm::vec3 color, float intensity);
            LightSource(glm::vec3 position, glm::vec3 color, glm::vec3 intensity);

            glm::vec3 get_position();
            glm::vec3 get_color();

            glm::vec3 get_intensity();
            float get_intensity_ambient();
            float get_intensity_diffuse();
            float get_intensity_specular();

            void set_intensity(glm::vec3 new_intensity);
            void set_intensity(float new_intensity);
            void set_intensity_ambient(float new_ambient);
            void set_intensity_diffuse(float new_diffuse);
            void set_intensity_specular(float new_specular);
            
        protected:
            glm::vec3 m_position;
            glm::vec3 m_color;
            glm::vec3 m_intensity; // {ambient, diffuse, specular}
    };
}