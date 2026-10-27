#pragma once
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glm/vec2.hpp"
#include <functional>
#include "Events/Event.hpp"

namespace Core {
    struct WindowConfig {
        unsigned width;
        unsigned height;
        std::string title;

        using EventCallbackFn = std::function<void(Event&)>; 
        EventCallbackFn event_callback;
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
            WindowConfig& getConfig();
            glm::vec2 getFramebufferSize() const;
            GLFWwindow* getHandle();
            
            void raiseEvent(Event& event);

        private:

            WindowConfig m_window_config;

            GLFWwindow* m_handle = nullptr;
    };
}