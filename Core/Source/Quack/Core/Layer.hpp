#pragma once
#include "Events/Event.hpp"
#include <memory>

namespace Core {
    class Layer {
        public:
            virtual void onEvent(Event& event) {}
            virtual void onUpdate(float dt) {}
            virtual void onRender() {}
            
            template<std::derived_from<Layer> LT, typename ... Args>
            void TransitionTo(Args&& ... args) {
                QueueTransition(std::move(std::make_unique<LT>(std::forward<Args>(args) ...)));
            }

        private:
            void QueueTransition(std::unique_ptr<Layer> layer);
    };
}