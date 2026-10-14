#pragma once
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glm/vec2.hpp"

namespace Core {
    struct WindowConfig {
        unsigned width;
        unsigned height;
        std::string title;
    };
    
    class Window {
        public:
            Window(const WindowConfig& config);
            ~Window();
            Window(const Window& other) = delete; // copy constructor
            Window& operator=(const Window& other) = delete; // copy assignment operator
            Window(const Window&& other) = delete; // move constructor
            Window& operator=(const Window&& other) = delete; // move assignment operator
            
            void create();
            void update();
            void destroy();
            WindowConfig& get_config();
            glm::vec2 get_framebuffer_size() const;
            GLFWwindow* get_handle();

        private:

            const WindowConfig& m_window_config;

            GLFWwindow* m_glfw_window = nullptr;
    };
}