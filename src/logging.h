#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <vector>
#include <functional>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& Instance();

    void Initialize(const std::wstring& logDir);
    void SetLogLevel(LogLevel level);
    void EnableDebug(bool enable);

    void Log(LogLevel level, const std::string& message);
    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);

    std::vector<std::string> GetRecentLogs(size_t count = 100) const;
    std::wstring GetLogPath() const;

    using LogCallback = std::function<void(LogLevel, const std::string&)>;
    void SetCallback(LogCallback callback);

private:
    Logger() = default;
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string LevelToString(LogLevel level) const;
    std::string CurrentTimestamp() const;

    std::wstring m_logPath;
    std::ofstream m_fileStream;
    LogLevel m_minLevel = LogLevel::Debug;
    bool m_debugEnabled = false;
    bool m_initialized = false;
    mutable std::recursive_mutex m_mutex;
    LogCallback m_callback;
    std::vector<std::string> m_recentLogs;
    static constexpr size_t MAX_RECENT_LOGS = 1000;
};
