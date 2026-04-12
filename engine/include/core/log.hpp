#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <memory>
#include <vector>
#include <string_view>

namespace engine::core {

enum class LogLevel {
    Info,
    Warning,
    Error,
    Critical
};

enum class LogCategory {
    Core,
    Platform,
    Render,
    Assets,
    Scene
};

struct LogRecord {
    LogLevel level;
    LogCategory category;
    std::string message;
};

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(const LogRecord& record) = 0;
};

class ConsoleSink final : public LogSink {
public:
    void write(const LogRecord& record) override;
};

class Logger final {
public:
    void add_sink(std::unique_ptr<LogSink> sink);

    void info(LogCategory category, std::string_view message);
    void warning(LogCategory category, std::string_view message);
    void error(LogCategory category, std::string_view message);
    void critical(LogCategory category, std::string_view message);

private:
    void write(LogLevel level, LogCategory category, std::string_view message);

    std::vector<std::unique_ptr<LogSink>> sinks;
};

} // namespace engine::core

#endif // LOG_HPP