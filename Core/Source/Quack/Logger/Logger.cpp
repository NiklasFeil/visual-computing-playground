#include "Logger/Logger.hpp"
#include <iostream>
#include <assert.h>
#include <print>
#include "Core/Application.hpp"
#include <format>

namespace Core {

    static Logger* s_Logger = nullptr;

    void Logger::create() {
        s_Logger = this;
        auto gui = Application::Get().getGui();
        gui->createGuiWindow("Log");
    }

    std::string to_string(LogLevel level) {
        switch (level) {
            case LogLevel::Info:
                return "Info";
            case LogLevel::Warning:
                return "Warning";
            case LogLevel::Error:
                return "Error"; 
            default:
                assert(false), "call of to_string function with invalid LogLevel";
        }
    }

    void Logger::Log(std::string content, LogLevel level) {
        
        LogMessage& msg = s_Logger->m_log_messages.emplace_back(std::move(content), std::move(level));

        const std::string to_print = std::format("{}: {}", to_string(msg.level), msg.content);
        std::println("{}", to_print);
        
        auto gui = Application::Get().getGui();
        GuiWindow& log_window = gui->getGuiWindow("Log");

        log_window.addWidget<Text>(std::format("logger_message_{}", s_Logger->m_log_messages.size()), to_print);
        
    }
}