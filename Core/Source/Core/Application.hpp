#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Layer.hpp"
#include "Window.hpp"
#include "glm/vec2.hpp"

namespace Core {
    struct ApplicationConfig {
        std::string name;
        WindowConfig window_config;
    };
    
    class Application {
        public:
            Application(const ApplicationConfig& config);
            ~Application();
            Application(const Application& other) = delete; // copy constructor
            Application& operator=(const Application& other) = delete; // copy assignment operator
            Application(const Application&& other) = delete; // move constructor
            Application& operator=(const Application&& other) = delete; // move assignment operator

            template <typename LT>
            requires(std::is_base_of_v<Layer, LT>)
            void push_layer() {
                m_layer_stack.push_back(std::make_unique<LT>());
            }

            template <typename LT>
            requires(std::is_base_of_v<Layer, LT>)
            LT* get_layer() {
                for (const auto& layer : m_layer_stack) {
                    if (auto casted = dynamic_cast<LT*>(layer.get())) {
                        return casted;
                    }
                }

                return nullptr;
            }

            glm::vec2 get_framebuffer_size() const;

            std::shared_ptr<Window> get_window() const;

            static Application& get();

            static float get_time();
            
            void run();
            void stop();

            
        private:

            void raise_event(Event& event);

            std::vector<std::unique_ptr<Layer>> m_layer_stack;
            ApplicationConfig m_app_config;
            std::shared_ptr<Window> m_window;
            bool m_is_running;
    };
}