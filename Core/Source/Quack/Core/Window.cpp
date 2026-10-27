#include "Core/Window.hpp"
#include <iostream>
#include "Events/InputEvent.hpp"
#include "Events/WindowEvent.hpp"

namespace Core {
    Window::Window(const WindowConfig& config) 
    : m_window_config(config) {}

    Window::~Window() {
        std::cout << "Terminating Window" << std::endl;
        destroy();
    }

    GLFWwindow* Window::getHandle() {
        return m_handle;
    };

    void Window::create() {

        m_handle = glfwCreateWindow(
            m_window_config.width, 
            m_window_config.height, 
            m_window_config.title.c_str(), 
            NULL, NULL);
        
        if (!m_handle) {
            // Raise error
            // todo: Proper exception system
            std::cout << "GLFW Window could not be initialized properly" << std::endl;
            assert(false);
        }

        glfwMakeContextCurrent(m_handle);

        glfwSetWindowUserPointer(m_handle, this); // Important so we can access the window object in the callbacks

        // Might be interesting later
        /*
        glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* glfw_window, int width, int height) {
            glViewport(0, 0, width, height);

            std::cout << "Framebuffer Size Callback: (" << width << ", " << height << ")" << std::endl;
        });
        */

        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* glfw_window, int width, int height) {
            Window& window = *((Window*)glfwGetWindowUserPointer(glfw_window));

            WindowResizeEvent event(width, height);
            window.raiseEvent(event);  
        });

        glfwSetKeyCallback(m_handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
				case GLFW_PRESS:
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, action == GLFW_REPEAT);
					window.raiseEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					window.raiseEvent(event);
					break;
				}
			}
		});

        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* handle, int button, int action, int mods)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					window.raiseEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					window.raiseEvent(event);
					break;
				}
			}
		});

        glfwSetScrollCallback(m_handle, [](GLFWwindow* handle, double x_offset, double y_offset)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			MouseScrolledEvent event(x_offset, y_offset);
			window.raiseEvent(event);
		});

        glfwSetCursorPosCallback(m_handle, [](GLFWwindow* handle, double x, double y)
		{
			Window& window = *((Window*)glfwGetWindowUserPointer(handle));

			MouseMovedEvent event(x, y);
			window.raiseEvent(event);
		});
    }

    void Window::update() {
        glfwSwapBuffers(m_handle);
        glfwPollEvents();
    }

    void Window::destroy() {
        if (m_handle)
            glfwDestroyWindow(m_handle);

        m_handle = nullptr;
    }

    glm::vec2 Window::getFramebufferSize() const {
        int width, height;
        glfwGetFramebufferSize(m_handle, &width, &height);
        return { width, height };
    }

    void Window::raiseEvent(Event& event) {
        if (m_window_config.event_callback) {
            m_window_config.event_callback(event);
        }
    }
}