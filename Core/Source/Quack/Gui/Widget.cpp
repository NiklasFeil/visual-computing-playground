#include "Widget.hpp"

namespace Core {

    void renderWidget(Widget& widget) {
        std::visit(VisitWidget(), widget);
    }
}