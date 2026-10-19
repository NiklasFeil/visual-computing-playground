#pragma once
#include "glm/vec2.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Application.hpp"
#include "ButtonMappings.hpp"
#include "KeyMappings.hpp"

namespace Core {

    static glm::vec2 s_last_mouse_pos; // pixel space

    class Input {
        public:

            static bool IsMouseButtonPressed(MouseButtonCode mb) { 
                auto const window = Application::get().get_window();
                return glfwGetMouseButton(window->get_handle(), mb) == GLFW_PRESS;
            };
            
            static bool IsKeyPressed(KeyCode key_code) {
                auto const window = Application::get().get_window();
                return glfwGetKey(window->get_handle(), key_code) == GLFW_PRESS;
            };
            
            static glm::vec2 GetMousePos() {
                auto window = Application::get().get_window();
                double x, y;
                glfwGetCursorPos(window->get_handle(), &x, &y);
                return {x, y};
            };
            
            static float GetMousePosX() {
                auto window = Application::get().get_window();
                double x, y;
                glfwGetCursorPos(window->get_handle(), &x, &y);
                return x;
            };
            
            static float GetMousePosY() {
                auto window = Application::get().get_window();
                double x, y;
                glfwGetCursorPos(window->get_handle(), &x, &y);
                return y;
            };

            static glm::vec2 GetLastMousePos() {
                return s_last_mouse_pos;    
            }

            static float GetLastMousePosX() {
                return s_last_mouse_pos.x;    
            }

            static float GetLastMousePosY() {
                return s_last_mouse_pos.y;    
            }

            static void SetLastMousePos(float x, float y) {
                s_last_mouse_pos = glm::vec2(x, y);
            }

    };
}