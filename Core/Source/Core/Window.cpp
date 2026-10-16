#include <GLAD/glad.h>
#include "Window.hpp"
#include <iostream>
#include "InputEvent.hpp"
#include "WindowEvent.hpp"

namespace Core {
    Window::Window(const WindowConfig& config) 
    : m_window_config(config) {}

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
            assert(false);
        }

        glfwMakeContextCurrent(m_glfw_window);

        glfwSetWindowUserPointer(m_glfw_window, this); // Important so we can access the window object in the callbacks

        // Might be interesting later
        /*
        glfwSetFramebufferSizeCallback(m_glfw_window, [](GLFWwindow* glfw_window, int width, int height) {
            glViewport(0, 0, width, height);

            std::cout << "Framebuffer Size Callback: (" << width << ", " << height << ")" << std::endl;
        });
        */

        glfwSetWindowSizeCallback(m_glfw_window, [](GLFWwindow* glfw_window, int width, int height) {
            Window& window = *((Window*)glfwGetWindowUserPointer(glfw_window));

            WindowResizeEvent event(width, height);
            window.raise_event(event);  
        });

        glfwSetKeyCallback(m_glfw_window, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
				case GLFW_PRESS:
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, action == GLFW_REPEAT);
					window.raise_event(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					window.raise_event(event);
					break;
				}
			}
		});

        glfwSetMouseButtonCallback(m_glfw_window, [](GLFWwindow* handle, int button, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					window.raise_event(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					window.raise_event(event);
					break;
				}
			}
		});

        glfwSetScrollCallback(m_glfw_window, [](GLFWwindow* handle, double x_offset, double y_offset)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			MouseScrolledEvent event(x_offset, y_offset);
			window.raise_event(event);
		});

        glfwSetCursorPosCallback(m_glfw_window, [](GLFWwindow* handle, double x, double y)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			MouseMovedEvent event(x, y);
			window.raise_event(event);
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

    glm::vec2 Window::get_framebuffer_size() const {
        int width, height;
        glfwGetFramebufferSize(m_glfw_window, &width, &height);
        return { width, height };
    }

    void Window::raise_event(Event& event) {
        if (m_window_config.event_callback) {
            m_window_config.event_callback(event);
        }
    }
}