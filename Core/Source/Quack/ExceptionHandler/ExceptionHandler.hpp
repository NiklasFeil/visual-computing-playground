#pragma once
#include "Logger/Logger.hpp"
#include <assert.h>

namespace Core {

    class ExceptionHandler {
        public:
            static void Assert(bool check, std::string message);
    };
}