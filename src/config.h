#pragma once

#include <string>
#include <unordered_map>

struct AppConfig {
    std::wstring appDataPath;
    std::wstring logPath;
    std::wstring configPath;
    std::wstring statePath;
    std::wstring frontendPath;

    int windowWidth = 1366;
    int windowHeight = 768;
    int minWidth = 1280;
    int minHeight = 720;
    bool startMaximized = false;
    bool devModeEnabled = false;
    bool mockDataEnabled = false;

    std::wstring defaultLanguage = L"en-US";
    std::wstring defaultRegion = L"US";
};

class ConfigManager {
public:
    static ConfigManager& Instance();

    bool Initialize();
    bool Load();
    bool Save();

    AppConfig& GetConfig() { return m_config; }
    const AppConfig& GetConfig() const { return m_config; }

    void SetDevMode(bool enabled);
    void SetMockData(bool enabled);
    bool IsDevMode() const { return m_config.devModeEnabled; }
    bool IsMockData() const { return m_config.mockDataEnabled; }

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    void ApplyDefaults();

    AppConfig m_config;
    std::wstring m_configFilePath;
};
