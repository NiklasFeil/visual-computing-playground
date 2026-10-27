#pragma once

#include "Core/Application.hpp"
#include "glm/vec2.hpp"

namespace Core {

    class Utils {
        public:

            static glm::vec2 pixel_to_nds(unsigned int pixel_x, unsigned int pixel_y) {
                glm::vec2 dim = Core::Application::Get().getFramebufferSize();

                // Mirror y-axis and remove pixel count
                float x = static_cast<float>(pixel_x) / static_cast<float>(dim.x);
                float y = -static_cast<float>(pixel_y) / static_cast<float>(dim.y);

                // Shift origin from upper left corner to screen center
                x = 2.0f * x - 1.0f;
                y = 2.0f * y + 1.0f;

                return {x, y};
            }

    };

}