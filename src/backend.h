#pragma once

#include <string>
#include <memory>
#include <functional>
#include "bridgemessage.h"

class MessageHandler;

class BackendService {
public:
    BackendService();
    ~BackendService();

    bool Initialize();
    void Shutdown();

    std::string ProcessCommand(const BridgeMessage& message);
    std::string GetStateJson() const;
    std::string GetLogsAsJson() const;

    bool IsDevModeEnabled() const;
    void SetDevModeEnabled(bool enabled);
    bool IsMockDataEnabled() const;
    void SetMockDataEnabled(bool enabled);

    std::shared_ptr<MessageHandler> GetHandler() const { return m_handler; }

private:
    std::string HandleNavigateTo(const BridgeMessage& msg);
    std::string HandleSetRegion(const BridgeMessage& msg);
    std::string HandleSetKeyboardLayout(const BridgeMessage& msg);
    std::string HandleSetKeyboardLayoutSecondary(const BridgeMessage& msg);
    std::string HandleSetNetworkConfig(const BridgeMessage& msg);
    std::string HandleAcceptLicense(const BridgeMessage& msg);
    std::string HandleSetDeviceName(const BridgeMessage& msg);
    std::string HandleSetAccountInfo(const BridgeMessage& msg);
    std::string HandleSetPassword(const BridgeMessage& msg);
    std::string HandleSetPrivacySettings(const BridgeMessage& msg);
    std::string HandleSetCustomization(const BridgeMessage& msg);
    std::string HandleSubmitAll(const BridgeMessage& msg);
    std::string HandleGetState(const BridgeMessage& msg);
    std::string HandleReset(const BridgeMessage& msg);
    std::string HandleExportState(const BridgeMessage& msg);
    std::string HandleImportState(const BridgeMessage& msg);
    std::string HandleEnableDevMode(const BridgeMessage& msg);
    std::string HandleDisableDevMode(const BridgeMessage& msg);
    std::string HandleSkipToPage(const BridgeMessage& msg);

    static std::string SimulateUserCreation(const std::string& username,
                                             const std::string& password);
    static std::string SimulateDeviceNaming(const std::string& name);
    static std::string SimulatePrivacyConfig(const std::string& settings);
    static std::string SimulateSetupCompletion();

    std::shared_ptr<MessageHandler> m_handler;
    bool m_initialized = false;
};
