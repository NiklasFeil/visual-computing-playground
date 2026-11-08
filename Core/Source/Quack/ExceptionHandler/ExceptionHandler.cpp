#include "ExceptionHandler/ExceptionHandler.hpp"

namespace Core {

    void ExceptionHandler::Assert(bool check, std::string message) {
        if (!check) {
            Logger::Log(std::move(message), LogLevel::Error);
            assert(false), message;
        }
    }
}