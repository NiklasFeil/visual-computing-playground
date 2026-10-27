#include "GLAD/glad.h"
#include "Core/Application.hpp"
#include "Events/Input.hpp"
#include <iostream>
#include "assert.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ranges>
#include <glm/common.hpp>

namespace Core {

    // declare static so it's only accessable in this translation unit. Without it, one could use extern to access it directly without get()
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationConfig& config) 
    : m_app_config(config) {
        s_Application = this;
        
        // Init GLFW
        #if defined(__linux__)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); // Needs to be X11 because wayland does not support ImGui dragging windows out of viewport.
        #endif

        if(!glfwInit()) {
            // TODO: Throw error
            std::cout << "GLFW could not be initialized" << std::endl;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_app_config.window_config.event_callback = [this](Event& event) { raiseEvent(event); };

        // Create Window
        m_window = std::make_unique<Window>(m_app_config.window_config);
        m_window->create();

        // Init GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glViewport(0, 0, m_app_config.window_config.width, m_app_config.window_config.height);
    }

    Application::~Application() {
        std::cout << "Terminating Application" << std::endl;
        s_Application = nullptr;
        glfwTerminate();
        m_window->destroy();
        for (auto& l : m_layer_stack) {
            l.reset();
        }
        m_layer_stack.clear();
    }
    
    void Application::run() {

        m_is_running = true;

        float last_time = GetTime();

        while(m_is_running) {

            float curr_time = GetTime();
            float dt = glm::clamp(curr_time - last_time, 0.001f, 0.1f);
            last_time = curr_time; 

            for (const auto& layer : m_layer_stack) {
                layer->onUpdate(dt);
            }

            for (const auto& layer : m_layer_stack) {
                layer->onRender();
            }

            m_window->update();

            // Design choice: Rather here or in event mouseMoved? 
            Input::SetLastMousePos(Input::GetMousePosX(), Input::GetMousePosY());

            if(glfwWindowShouldClose(m_window->getHandle())) {
                m_window->destroy();
                m_is_running = false;
            }
        }
    }

    void Application::stop() {
        m_is_running = false;
    }

    glm::vec2 Application::getFramebufferSize() const {
        return m_window->getFramebufferSize();
    }

    std::shared_ptr<Window> Application::getWindow() const {
        assert(m_window);
        return m_window;
    }

    std::vector<std::unique_ptr<Layer>>& Application::getLayerStack() {
        return m_layer_stack;
    }

    Application& Application::Get() {
        assert(s_Application);
        return *s_Application;
    }


    float Application::GetTime() {
        return glfwGetTime();
    }


    void Application::raiseEvent(Event& event) {

        for (auto& layer : std::views::reverse(m_layer_stack)) {
            layer->onEvent(event);
            if (event.handled) break;
        }
    }
}