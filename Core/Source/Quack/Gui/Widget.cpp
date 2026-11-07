#include "Widget.hpp"

namespace Core {

    Widget::Widget() {
        id = id_counter++;
    }

    /*
    Widget::Widget(const Widget& other) {
        id = other.id;
        data = other.data;
        label = other.label;
    }

    Widget& Widget::operator=(const Widget& other) {
        id = other.id;
        data = other.data;
        label = other.label;
        return *this;
    }
    */

    Widget::Widget(const Widget& other) = default;
    Widget& Widget::operator=(const Widget& other) = default;
}