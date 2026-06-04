#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <wil/com.h>
#include <WebView2.h>

enum class BridgeCommand {
    Unknown,
    NavigateTo,
    GoBack,
    GoNext,
    Cancel,
    Reset,

    SetRegion,
    SetKeyboardLayout,
    SetKeyboardLayoutSecondary,
    SetNetworkConfig,
    AcceptLicense,
    SetDeviceName,
    SetAccountInfo,
    SetPassword,
    SetPrivacySettings,
    SetCustomization,

    SubmitAll,

    GetState,
    SetState,
    ExportState,
    ImportState,
    GetLogs,

    EnableDevMode,
    DisableDevMode,
    SkipToPage,
    GetWallpaper,

    Notify
};

struct BridgeMessage {
    BridgeCommand command = BridgeCommand::Unknown;
    std::string payload;
    std::string requestId;

    bool IsDevCommand() const;
    bool IsNavigationCommand() const;
    bool IsDataCommand() const;
    bool IsSystemCommand() const;
};

class MessageHandler {
public:
    using HandlerCallback = std::function<std::string(const BridgeMessage&)>;

    MessageHandler();

    void HandleMessage(const std::string& messageJson,
                       ICoreWebView2* webview);

    void RegisterHandler(BridgeCommand command, HandlerCallback handler);
    void SetDefaultHandler(HandlerCallback handler);

    std::string CallHandler(BridgeCommand command, const BridgeMessage& msg);

    static BridgeMessage ParseMessage(const std::string& json);
    static std::string CreateResponse(const std::string& requestId,
                                       bool success,
                                       const std::string& data = "");
    static std::string CreateError(const std::string& requestId,
                                    const std::string& error);

private:
    std::unordered_map<BridgeCommand, HandlerCallback> m_handlers;
    HandlerCallback m_defaultHandler;
};
