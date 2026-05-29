#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Layer.hpp"
#include "Window.hpp"

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

            void run();
            void stop();

            static Application& get();
            std::shared_ptr<Window> get_window() const;
            static float get_time();
            
        private:
            std::vector<std::unique_ptr<Layer>> m_layer_stack;
            const ApplicationConfig& m_app_config;
            std::shared_ptr<Window> m_window;
            bool m_is_running;
    };
}