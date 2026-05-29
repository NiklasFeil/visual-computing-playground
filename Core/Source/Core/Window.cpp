#include <GLAD/glad.h>
#include "Window.hpp"
#include <iostream>

namespace Core {
    Window::Window(const WindowConfig& config) 
    : m_window_config(config) {

    }

    Window::~Window() {
        std::cout << "Terminating Window" << std::endl;
        destroy();
    }


    GLFWwindow* Window::get_handle() {
        return m_glfw_window;
    };

    void Window::create() {

        m_glfw_window = glfwCreateWindow(
            m_window_config.width, 
            m_window_config.height, 
            m_window_config.title.c_str(), 
            NULL, NULL);
        
        if (!m_glfw_window) {
            // Raise error
            // todo: Proper exception system
            std::cout << "GLFW Window could not be initialized properly" << std::endl;
        }

        glfwMakeContextCurrent(m_glfw_window);

        glfwSetFramebufferSizeCallback(m_glfw_window, [](GLFWwindow* glfw_window, int width, int height) {
            glViewport(0, 0, width, height);
        });

    }

    void Window::update() {
        glfwSwapBuffers(m_glfw_window);
        glfwPollEvents();
    }

    void Window::destroy() {
        if (m_glfw_window)
            glfwDestroyWindow(m_glfw_window);

        m_glfw_window = nullptr;
    }
}