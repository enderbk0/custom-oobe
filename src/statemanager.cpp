#include "statemanager.h"
#include "logging.h"
#include "jsonutils.h"
#include <fstream>
#include <sstream>
#include <shlwapi.h>

StateManager& StateManager::Instance() {
    static StateManager instance;
    return instance;
}

void StateManager::Initialize(const std::wstring& stateDir) {
    m_stateFilePath = stateDir + L"\\oobe_state.json";
    LoadFromDisk();
    Logger::Instance().Info("StateManager initialized");
}

void StateManager::SetString(const std::string& key, const std::string& value) {
    auto it = m_state.find(key);
    if (it != m_state.end() && it->second == value) return;

    m_state[key] = value;
    Logger::Instance().Debug("State set: " + key + " = " + value);
    SaveToDisk();
    if (m_onChanged) m_onChanged(key);
}

void StateManager::SetInt(const std::string& key, int value) {
    SetString(key, std::to_string(value));
}

void StateManager::SetBool(const std::string& key, bool value) {
    SetString(key, value ? "true" : "false");
}

std::string StateManager::GetString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_state.find(key);
    if (it != m_state.end()) return it->second;
    return defaultValue;
}

int StateManager::GetInt(const std::string& key, int defaultValue) const {
    auto it = m_state.find(key);
    if (it != m_state.end()) {
        try { return std::stoi(it->second); } catch (...) {}
    }
    return defaultValue;
}

bool StateManager::GetBool(const std::string& key, bool defaultValue) const {
    auto it = m_state.find(key);
    if (it != m_state.end()) {
        return it->second == "true";
    }
    return defaultValue;
}

bool StateManager::HasKey(const std::string& key) const {
    return m_state.find(key) != m_state.end();
}

void StateManager::Reset() {
    m_state.clear();
    SetCurrentPage("welcome");
    SaveToDisk();
    Logger::Instance().Info("State reset to defaults");
}

int StateManager::GetCurrentPageIndex() const {
    return GetInt("pageIndex", 0);
}

void StateManager::AdvancePage() {
    int idx = GetCurrentPageIndex();
    SetInt("pageIndex", idx + 1);
}

bool StateManager::ExportState(const std::wstring& filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;
    file << ToJson();
    Logger::Instance().Info("State exported to file");
    return true;
}

bool StateManager::ImportState(const std::wstring& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    m_state.clear();
    std::stringstream buffer;
    buffer << file.rdbuf();

    Logger::Instance().Info("State imported from file");
    SaveToDisk();
    return true;
}

void StateManager::SetDevMode(bool enabled) {
    SetBool("devMode", enabled);
}

std::string StateManager::ToJson() const {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (const auto& [key, value] : m_state) {
        if (!first) ss << ",";
        first = false;
        ss << "\"" << key << "\":\"" << value << "\"";
    }
    ss << "}";
    return ss.str();
}

void StateManager::SetOnChanged(StateChangedCallback callback) {
    m_onChanged = std::move(callback);
}

std::unordered_map<std::string, std::string> StateManager::GetAll() const {
    return m_state;
}

bool StateManager::SaveToDisk() const {
    if (m_stateFilePath.empty()) return false;

    std::ofstream file(m_stateFilePath);
    if (!file.is_open()) {
        Logger::Instance().Error("Failed to save state to " +
            std::string(m_stateFilePath.begin(), m_stateFilePath.end()));
        return false;
    }
    file << ToJson();
    return true;
}

bool StateManager::LoadFromDisk() {
    if (m_stateFilePath.empty()) return false;

    std::ifstream file(m_stateFilePath);
    if (!file.is_open()) return false;

    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        if (content.empty()) return true;

        JsonUtils::ParseKeyValuePairs(content, m_state);
        Logger::Instance().Info("State loaded from disk (" +
            std::to_string(m_state.size()) + " keys)");
    } catch (const std::exception& e) {
        Logger::Instance().Warning(std::string("Failed to parse state file: ") + e.what());
    }

    return true;
}
