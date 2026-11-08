#pragma once
#include <string>
#include <vector>

namespace Core {

    enum class LogLevel {
        Info = 0,
        Warning,
        Error
    };

    struct LogMessage {
        std::string content;
        LogLevel level;
    };

    class Logger {

        public:
            Logger() {}

            void create();

            static void Log(std::string message, LogLevel level);

        private:

            std::vector<LogMessage> m_log_messages;
    };
}