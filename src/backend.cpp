#include "backend.h"
#include "statemanager.h"
#include "config.h"
#include "logging.h"
// TODO(Phase2): Include <oobenotification.h> for real OOBEComplete() detection
#include <sstream>
#include <chrono>
#include <thread>

BackendService::BackendService()
    : m_handler(std::make_shared<MessageHandler>()) {
}

BackendService::~BackendService() {
    Shutdown();
}

bool BackendService::Initialize() {
    Logger::Instance().Info("BackendService initializing");

    m_handler->RegisterHandler(BridgeCommand::NavigateTo,
        [this](const BridgeMessage& msg) { return HandleNavigateTo(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetRegion,
        [this](const BridgeMessage& msg) { return HandleSetRegion(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetKeyboardLayout,
        [this](const BridgeMessage& msg) { return HandleSetKeyboardLayout(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetKeyboardLayoutSecondary,
        [this](const BridgeMessage& msg) { return HandleSetKeyboardLayoutSecondary(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetNetworkConfig,
        [this](const BridgeMessage& msg) { return HandleSetNetworkConfig(msg); });
    m_handler->RegisterHandler(BridgeCommand::AcceptLicense,
        [this](const BridgeMessage& msg) { return HandleAcceptLicense(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetDeviceName,
        [this](const BridgeMessage& msg) { return HandleSetDeviceName(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetAccountInfo,
        [this](const BridgeMessage& msg) { return HandleSetAccountInfo(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetPassword,
        [this](const BridgeMessage& msg) { return HandleSetPassword(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetPrivacySettings,
        [this](const BridgeMessage& msg) { return HandleSetPrivacySettings(msg); });
    m_handler->RegisterHandler(BridgeCommand::SetCustomization,
        [this](const BridgeMessage& msg) { return HandleSetCustomization(msg); });
    m_handler->RegisterHandler(BridgeCommand::SubmitAll,
        [this](const BridgeMessage& msg) { return HandleSubmitAll(msg); });
    m_handler->RegisterHandler(BridgeCommand::GetState,
        [this](const BridgeMessage& msg) { return HandleGetState(msg); });
    m_handler->RegisterHandler(BridgeCommand::GetLogs,
        [this](const BridgeMessage& msg) {
            return MessageHandler::CreateResponse(msg.requestId, true,
                GetLogsAsJson());
        });
    m_handler->RegisterHandler(BridgeCommand::Reset,
        [this](const BridgeMessage& msg) { return HandleReset(msg); });
    m_handler->RegisterHandler(BridgeCommand::ExportState,
        [this](const BridgeMessage& msg) { return HandleExportState(msg); });
    m_handler->RegisterHandler(BridgeCommand::ImportState,
        [this](const BridgeMessage& msg) { return HandleImportState(msg); });
    m_handler->RegisterHandler(BridgeCommand::EnableDevMode,
        [this](const BridgeMessage& msg) { return HandleEnableDevMode(msg); });
    m_handler->RegisterHandler(BridgeCommand::DisableDevMode,
        [this](const BridgeMessage& msg) { return HandleDisableDevMode(msg); });
    m_handler->RegisterHandler(BridgeCommand::SkipToPage,
        [this](const BridgeMessage& msg) { return HandleSkipToPage(msg); });

    Logger::Instance().Info("BackendService handlers registered");

    if (ConfigManager::Instance().IsDevMode()) {
        SetDevModeEnabled(true);
    }
    if (ConfigManager::Instance().IsMockData()) {
        SetMockDataEnabled(true);
    }

    m_initialized = true;
    Logger::Instance().Info("BackendService initialized successfully");
    return true;
}

void BackendService::Shutdown() {
    if (m_initialized) {
        Logger::Instance().Info("BackendService shutting down");
        m_initialized = false;
    }
}

std::string BackendService::ProcessCommand(const BridgeMessage& message) {
    auto it = GetHandler()->m_handlers.find(message.command);
    if (it != GetHandler()->m_handlers.end()) {
        return it->second(message);
    }
    return MessageHandler::CreateError(message.requestId, "Unknown command");
}

std::string BackendService::GetStateJson() const {
    return StateManager::Instance().ToJson();
}

std::string BackendService::GetLogsAsJson() const {
    auto logs = Logger::Instance().GetRecentLogs(50);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < logs.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "\"" << logs[i] << "\"";
    }
    ss << "]";
    return ss.str();
}

bool BackendService::IsDevModeEnabled() const {
    return StateManager::Instance().IsDevMode();
}

void BackendService::SetDevModeEnabled(bool enabled) {
    StateManager::Instance().SetDevMode(enabled);
    ConfigManager::Instance().SetDevMode(enabled);
}

bool BackendService::IsMockDataEnabled() const {
    return ConfigManager::Instance().IsMockData();
}

void BackendService::SetMockDataEnabled(bool enabled) {
    ConfigManager::Instance().SetMockData(enabled);
}

std::string BackendService::HandleNavigateTo(const BridgeMessage& msg) {
    StateManager::Instance().SetCurrentPage(msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "navigated to " + msg.payload);
}

std::string BackendService::HandleSetRegion(const BridgeMessage& msg) {
    StateManager::Instance().SetString("region", msg.payload);
    Logger::Instance().Info("Region set: " + msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Region set to " + msg.payload);
}

std::string BackendService::HandleSetKeyboardLayout(const BridgeMessage& msg) {
    StateManager::Instance().SetString("keyboardLayout", msg.payload);
    Logger::Instance().Info("Keyboard layout set: " + msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Keyboard layout set to " + msg.payload);
}

std::string BackendService::HandleSetKeyboardLayoutSecondary(const BridgeMessage& msg) {
    StateManager::Instance().SetString("keyboardLayoutSecondary", msg.payload);
    Logger::Instance().Info("Secondary keyboard layout set: " + msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Secondary keyboard layout set to " + msg.payload);
}

std::string BackendService::HandleSetNetworkConfig(const BridgeMessage& msg) {
    StateManager::Instance().SetString("networkConfig", msg.payload);
    Logger::Instance().Info("Network configuration saved");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleAcceptLicense(const BridgeMessage& msg) {
    StateManager::Instance().SetBool("licenseAccepted", true);
    Logger::Instance().Info("License agreement accepted");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleSetDeviceName(const BridgeMessage& msg) {
    StateManager::Instance().SetString("deviceName", msg.payload);
    Logger::Instance().Info("Device name set: " + msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Device name set to " + msg.payload);
}

std::string BackendService::HandleSetAccountInfo(const BridgeMessage& msg) {
    StateManager::Instance().SetString("accountInfo", msg.payload);
    Logger::Instance().Info("Account info saved");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleSetPassword(const BridgeMessage& msg) {
    StateManager::Instance().SetString("passwordSet", "true");
    Logger::Instance().Info("Password configured");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleSetPrivacySettings(const BridgeMessage& msg) {
    StateManager::Instance().SetString("privacySettings", msg.payload);
    Logger::Instance().Info("Privacy settings saved");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleSetCustomization(const BridgeMessage& msg) {
    StateManager::Instance().SetString("customization", msg.payload);
    Logger::Instance().Info("Customization settings saved");
    return MessageHandler::CreateResponse(msg.requestId, true);
}

std::string BackendService::HandleSubmitAll(const BridgeMessage& msg) {
    Logger::Instance().Info("=== BEGINNING SIMULATED SETUP ACTIONS ===");

    std::string name = StateManager::Instance().GetString("deviceName", "DESKTOP-USER");
    std::string user = StateManager::Instance().GetString("accountInfo", "User");
    std::string pass = StateManager::Instance().GetBool("passwordSet") ? "[SET]" : "[NONE]";

    SimulateDeviceNaming(name);
    SimulateUserCreation(user, pass);
    SimulatePrivacyConfig(StateManager::Instance().GetString("privacySettings"));
    SimulateSetupCompletion();

    StateManager::Instance().SetBool("setupComplete", true);

    Logger::Instance().Info("=== SIMULATED SETUP ACTIONS COMPLETE ===");

    return MessageHandler::CreateResponse(msg.requestId, true,
        "Setup complete");
}

std::string BackendService::HandleGetState(const BridgeMessage& msg) {
    return MessageHandler::CreateResponse(msg.requestId, true,
        GetStateJson());
}

std::string BackendService::HandleReset(const BridgeMessage& msg) {
    StateManager::Instance().Reset();
    Logger::Instance().Info("State reset via command");
    return MessageHandler::CreateResponse(msg.requestId, true,
        "State reset to defaults");
}

std::string BackendService::HandleExportState(const BridgeMessage& msg) {
    std::string state = GetStateJson();
    Logger::Instance().Info("State exported via command");
    return MessageHandler::CreateResponse(msg.requestId, true, state);
}

std::string BackendService::HandleImportState(const BridgeMessage& msg) {
    Logger::Instance().Info("State import requested");
    return MessageHandler::CreateResponse(msg.requestId, true,
        "State import requires file path");
}

std::string BackendService::HandleEnableDevMode(const BridgeMessage& msg) {
    SetDevModeEnabled(true);
    Logger::Instance().Info("Developer mode enabled via command");
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Developer mode enabled");
}

std::string BackendService::HandleDisableDevMode(const BridgeMessage& msg) {
    SetDevModeEnabled(false);
    Logger::Instance().Info("Developer mode disabled via command");
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Developer mode disabled");
}

std::string BackendService::HandleSkipToPage(const BridgeMessage& msg) {
    StateManager::Instance().SetCurrentPage(msg.payload);
    Logger::Instance().Info("Skip to page: " + msg.payload);
    return MessageHandler::CreateResponse(msg.requestId, true,
        "Skipped to " + msg.payload);
}

std::string BackendService::SimulateUserCreation(const std::string& username,
                                                   const std::string& password) {
    Logger::Instance().Info("SIMULATED: Creating local user account: " + username);
    Logger::Instance().Info("SIMULATED: Password complexity verified");
    Logger::Instance().Info("SIMULATED: User profile directory created");
    return "User account simulated: " + username;
}

std::string BackendService::SimulateDeviceNaming(const std::string& name) {
    Logger::Instance().Info("SIMULATED: Device named: " + name);
    Logger::Instance().Info("SIMULATED: Computer name registered");
    return "Device named: " + name;
}

std::string BackendService::SimulatePrivacyConfig(const std::string& settings) {
    Logger::Instance().Info("SIMULATED: Privacy settings applied: " + settings);
    Logger::Instance().Info("SIMULATED: Telemetry configured");
    Logger::Instance().Info("SIMULATED: Location access configured");
    Logger::Instance().Info("SIMULATED: Advertising ID configured");
    return "Privacy settings applied";
}

std::string BackendService::SimulateSetupCompletion() {
    // TODO(Phase3): Replace with real OOBE completion via OOBEComplete() API
    Logger::Instance().Info("SIMULATED: Setup finalizing...");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Logger::Instance().Info("SIMULATED: Finalizing configuration");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    Logger::Instance().Info("SIMULATED: Setup complete");
    return "Setup finalized";
}
