#include "config.h"
#include "logging.h"
#include "jsonutils.h"
#include <shlobj.h>
#include <shlwapi.h>
#include <fstream>
#include <sstream>

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::Initialize() {
    ApplyDefaults();

    wchar_t appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA,
                                   nullptr, SHGFP_TYPE_CURRENT, appDataPath))) {
        m_config.appDataPath = std::wstring(appDataPath) + L"\\CustomOOBE";
    } else {
        m_config.appDataPath = L".\\data";
    }

    SHCreateDirectoryExW(nullptr, m_config.appDataPath.c_str(), nullptr);

    m_config.logPath = m_config.appDataPath + L"\\logs";
    m_config.configPath = m_config.appDataPath + L"\\config";
    m_config.statePath = m_config.appDataPath + L"\\state";

    SHCreateDirectoryExW(nullptr, m_config.logPath.c_str(), nullptr);
    SHCreateDirectoryExW(nullptr, m_config.configPath.c_str(), nullptr);
    SHCreateDirectoryExW(nullptr, m_config.statePath.c_str(), nullptr);

    m_configFilePath = m_config.configPath + L"\\settings.json";

    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(nullptr, modulePath, MAX_PATH);
    std::wstring moduleDir(modulePath);
    size_t pos = moduleDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        moduleDir = moduleDir.substr(0, pos);
    }
    m_config.frontendPath = moduleDir + L"\\frontend";

    return Load();
}

bool ConfigManager::Load() {
    std::ifstream file(m_configFilePath);
    if (!file.is_open()) {
        Logger::Instance().Info("No config file found, using defaults");
        return true;
    }

    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        JsonUtils::JsonValue root = JsonUtils::Parse(content);
        if (root.type != JsonUtils::JsonValue::Type::Object) {
            Logger::Instance().Warning("Config file is not a JSON object");
            return true;
        }

        if (root.Has("windowWidth"))
            m_config.windowWidth = root.Get("windowWidth").AsInt(1366);
        if (root.Has("windowHeight"))
            m_config.windowHeight = root.Get("windowHeight").AsInt(768);
        if (root.Has("minWidth"))
            m_config.minWidth = root.Get("minWidth").AsInt(1280);
        if (root.Has("minHeight"))
            m_config.minHeight = root.Get("minHeight").AsInt(720);
        if (root.Has("startMaximized"))
            m_config.startMaximized = root.Get("startMaximized").AsBool(false);
        if (root.Has("devModeEnabled"))
            m_config.devModeEnabled = root.Get("devModeEnabled").AsBool(false);
        if (root.Has("mockDataEnabled"))
            m_config.mockDataEnabled = root.Get("mockDataEnabled").AsBool(false);

        Logger::Instance().Info("Configuration loaded from file");
    } catch (const std::exception& e) {
        Logger::Instance().Warning(std::string("Failed to parse config file: ") + e.what());
    }

    return true;
}

bool ConfigManager::Save() {
    std::ofstream file(m_configFilePath);
    if (!file.is_open()) {
        Logger::Instance().Error("Failed to open config file for writing");
        return false;
    }

    file << "{" << std::endl;
    file << "  \"windowWidth\": " << m_config.windowWidth << "," << std::endl;
    file << "  \"windowHeight\": " << m_config.windowHeight << "," << std::endl;
    file << "  \"minWidth\": " << m_config.minWidth << "," << std::endl;
    file << "  \"minHeight\": " << m_config.minHeight << "," << std::endl;
    file << "  \"startMaximized\": " << (m_config.startMaximized ? "true" : "false") << "," << std::endl;
    file << "  \"devModeEnabled\": " << (m_config.devModeEnabled ? "true" : "false") << "," << std::endl;
    file << "  \"mockDataEnabled\": " << (m_config.mockDataEnabled ? "true" : "false") << std::endl;
    file << "}" << std::endl;

    Logger::Instance().Info("Configuration saved");
    return true;
}

void ConfigManager::SetDevMode(bool enabled) {
    m_config.devModeEnabled = enabled;
    Logger::Instance().EnableDebug(enabled);
    Save();
}

void ConfigManager::SetMockData(bool enabled) {
    m_config.mockDataEnabled = enabled;
    Logger::Instance().Info(std::string("Mock data: ") + (enabled ? "enabled" : "disabled"));
    Save();
}

void ConfigManager::ApplyDefaults() {
    m_config.windowWidth = 1366;
    m_config.windowHeight = 768;
    m_config.minWidth = 1280;
    m_config.minHeight = 720;
    m_config.startMaximized = false;
    m_config.devModeEnabled = false;
    m_config.mockDataEnabled = false;
}
