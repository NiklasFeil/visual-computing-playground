#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Core/Layer.hpp"
#include "Core/Window.hpp"
#include "Logger/Logger.hpp"
#include "Gui/Gui.hpp"
#include "glm/vec2.hpp"

namespace Core {
    struct ApplicationConfig {
        std::string name;
        WindowConfig window_config;
        GuiConfig gui_config;
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
            void pushLayer() {
                std::unique_ptr<LT> layer = std::make_unique<LT>();
                layer->onAttach();
                m_layer_stack.push_back(std::move(layer));
            }

            template <typename LT>
            requires(std::is_base_of_v<Layer, LT>)
            LT* getLayer() {
                for (const auto& layer : m_layer_stack) {
                    if (auto casted = dynamic_cast<LT*>(layer.get())) {
                        return casted;
                    }
                }

                return nullptr;
            }

            glm::vec2 getFramebufferSize() const;

            std::shared_ptr<Gui> getGui() const;

            std::shared_ptr<Window> getWindow() const;
            
            std::vector<std::unique_ptr<Layer>>& getLayerStack();

            static Application& Get();

            static float GetTime();
            
            void run();
            void stop();

            
        private:

            void raiseEvent(Event& event);

            std::vector<std::unique_ptr<Layer>> m_layer_stack;
            ApplicationConfig m_app_config;

            std::shared_ptr<Gui> m_gui;
            std::shared_ptr<Window> m_window;
            std::shared_ptr<Logger> m_logger;
            
            bool m_is_running;
    };
}