#include "../include/core/log.hpp"

#include <iostream>
#include <utility>

namespace engine::core {

namespace {

const char* to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Info:
            return "info";
        case LogLevel::Warning:
            return "warning";
        case LogLevel::Error:
            return "error";
        case LogLevel::Critical:
            return "critical";
    }

    return "unknown";
}

const char* to_string(LogCategory category) {
    switch (category) {
        case LogCategory::Core:
            return "core";
        case LogCategory::Platform:
            return "platform";
        case LogCategory::Render:
            return "render";
        case LogCategory::Assets:
            return "assets";
        case LogCategory::Scene:
            return "scene";
    }

    return "unknown";
}

}  // namespace

void ConsoleSink::write(const LogRecord& record) {
    std::ostream& output =
        (record.level == LogLevel::Error || record.level == LogLevel::Critical)
            ? std::cerr
            : std::cout;

    output << "[" << to_string(record.level) << "] "
           << "[" << to_string(record.category) << "] "
           << record.message << '\n';
}

void Logger::add_sink(std::unique_ptr<LogSink> sink) {
    if (!sink) {
        return;
    }

    sinks.push_back(std::move(sink));
}

void Logger::info(LogCategory category, std::string_view message) {
    write(LogLevel::Info, category, message);
}

void Logger::warning(LogCategory category, std::string_view message) {
    write(LogLevel::Warning, category, message);
}

void Logger::error(LogCategory category, std::string_view message) {
    write(LogLevel::Error, category, message);
}

void Logger::critical(LogCategory category, std::string_view message) {
    write(LogLevel::Critical, category, message);
}

void Logger::write(LogLevel level, LogCategory category, std::string_view message) {
    LogRecord record {
        level,
        category,
        std::string(message)
    };

    for (const auto& sink : sinks) {
        sink->write(record);
    }
}

}  // namespace engine::core
