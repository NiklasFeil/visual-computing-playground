#pragma once
#include "Entity.hpp"

namespace Core {

    class Cuboid : public Entity {

        public:
            Cuboid(glm::vec3 pos, glm::vec3 dim, glm::vec3 color, bool inverted);

    };

};