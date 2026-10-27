#include "Core/Layer.hpp"
#include <memory>
#include "Core/Application.hpp"

namespace Core {
    void Layer::QueueTransition(std::unique_ptr<Layer> new_layer) {
        // TODO: Queue this instead of instantly removing the layer from the stack. For now, this is fine, but it produces undefined behaviour when called inside a function of the transitioning layer.
        auto& layer_stack = Application::Get().getLayerStack();

        for (auto& layer : layer_stack) {
            if (layer.get() == this) {
                layer = std::move(new_layer);
                return;
            }
        }
    }

}