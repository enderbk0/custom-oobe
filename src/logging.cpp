#include "logging.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <shlwapi.h>
#include <windows.h>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::Initialize(const std::wstring& logDir) {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::wstring dir = logDir;
    if (dir.back() != L'\\' && dir.back() != L'/') {
        dir += L'\\';
    }
    SHCreateDirectoryExW(nullptr, dir.c_str(), nullptr);

    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &tt);

    std::wstringstream ss;
    ss << dir << L"oobe_"
       << std::put_time(&tm, L"%Y%m%d_%H%M%S")
       << L".log";
    m_logPath = ss.str();

    m_fileStream.open(m_logPath, std::ios::out | std::ios::app);
    if (m_fileStream.is_open()) {
        Info("Logger initialized");
    }
}

void Logger::SetLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void Logger::EnableDebug(bool enable) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_debugEnabled = enable;
}

void Logger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (level < m_minLevel) return;
    if (level == LogLevel::Debug && !m_debugEnabled) return;

    std::string timestamp = CurrentTimestamp();
    std::string levelStr = LevelToString(level);
    std::string formatted = "[" + timestamp + "] [" + levelStr + "] " + message;

    m_recentLogs.push_back(formatted);
    if (m_recentLogs.size() > MAX_RECENT_LOGS) {
        m_recentLogs.erase(m_recentLogs.begin());
    }

    if (m_fileStream.is_open()) {
        m_fileStream << formatted << std::endl;
        m_fileStream.flush();
    }

    if (m_callback) {
        m_callback(level, formatted);
    }

    OutputDebugStringA((formatted + "\n").c_str());
}

void Logger::Debug(const std::string& message) {
    Log(LogLevel::Debug, message);
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::Info, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::Warning, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::Error, message);
}

std::vector<std::string> Logger::GetRecentLogs(size_t count) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_recentLogs.size() <= count) {
        return m_recentLogs;
    }
    return std::vector<std::string>(
        m_recentLogs.end() - static_cast<std::ptrdiff_t>(count),
        m_recentLogs.end()
    );
}

std::wstring Logger::GetLogPath() const {
    return m_logPath;
}

void Logger::SetCallback(LogCallback callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_callback = std::move(callback);
}

std::string Logger::LevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

std::string Logger::CurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &tt);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
