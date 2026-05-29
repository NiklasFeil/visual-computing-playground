#pragma once
#include "Event.hpp"

namespace Core {
    class Layer {
        public:
            virtual void onEvent(Event& event) {}
            virtual void onUpdate(float dt) {}
            virtual void onRender() {}
    };
}