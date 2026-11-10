#include "ExceptionHandler/ExceptionHandler.hpp"

namespace Core {

    void ExceptionHandler::Assert(bool check, std::string message) {
        if (!check) {
            Logger::Log(message, LogLevel::Error);
            assert(false), message;
        }
    }

    void ExceptionHandler::Throw(std::runtime_error error, std::string message) {
        Logger::Log(std::move(message), LogLevel::Error);
        throw error;
    }
}