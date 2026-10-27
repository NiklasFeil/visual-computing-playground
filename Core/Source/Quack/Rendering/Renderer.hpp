#pragma once
#include "glm/glm.hpp"
#include "GLAD/glad.h"

namespace Core {
    class Renderer {
        public:
            static void ClearColorBuffer(glm::vec3 color) {
                glClearColor(color.r, color.g, color.b, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            
            static void ClearDepthBuffer() {
                glClear(GL_DEPTH_BUFFER_BIT);
            }

            static void EnableBackFaceCulling() {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            }

            static void EnableFrontFaceCulling() {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
            }

            static void SetWindingOrderCW() {
                glFrontFace(GL_CW);
            }

            static void SetWindingOrderCCW() {
                glFrontFace(GL_CCW);
            }

            static void Draw(GLuint vao, unsigned int num_elements) {
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
    };
}