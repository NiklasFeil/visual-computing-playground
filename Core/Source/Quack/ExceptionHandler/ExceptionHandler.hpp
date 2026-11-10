#pragma once
#include "Logger/Logger.hpp"
#include <assert.h>
#include <stdexcept>

namespace Core {

    class ExceptionHandler {
        public:
            static void Assert(bool check, std::string message);
            static void Throw(std::runtime_error error, std::string message);
    };
}