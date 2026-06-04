#pragma once

#include <string>
#include <unordered_map>
#include <functional>

class StateManager {
public:
    static StateManager& Instance();

    void Initialize(const std::wstring& stateDir);

    void SetString(const std::string& key, const std::string& value);
    void SetInt(const std::string& key, int value);
    void SetBool(const std::string& key, bool value);

    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
    int GetInt(const std::string& key, int defaultValue = 0) const;
    bool GetBool(const std::string& key, bool defaultValue = false) const;

    bool HasKey(const std::string& key) const;

    void Reset();
    bool ExportState(const std::wstring& filePath) const;
    bool ImportState(const std::wstring& filePath);

    std::string GetCurrentPage() const { return GetString("currentPage", "welcome"); }
    void SetCurrentPage(const std::string& page) { SetString("currentPage", page); }

    int GetCurrentPageIndex() const;
    void AdvancePage();

    bool IsDevMode() const { return GetBool("devMode", false); }
    void SetDevMode(bool enabled);

    std::string ToJson() const;

    using StateChangedCallback = std::function<void(const std::string& key)>;
    void SetOnChanged(StateChangedCallback callback);

    std::unordered_map<std::string, std::string> GetAll() const;

private:
    StateManager() = default;
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    bool SaveToDisk() const;
    bool LoadFromDisk();

    std::wstring m_stateFilePath;
    std::unordered_map<std::string, std::string> m_state;
    StateChangedCallback m_onChanged;
};
