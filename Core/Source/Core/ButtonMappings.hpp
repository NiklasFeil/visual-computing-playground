#pragma once

namespace Core {

    using MouseButtonCode = unsigned char; // uint8_t

    namespace MouseButton {
        enum : MouseButtonCode {
            Left = 0,
            Right = 1,
            Middle = 2
        };
    }

}