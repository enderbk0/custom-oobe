#include "bridgemessage.h"
#include "logging.h"
#include "jsonutils.h"
#include <sstream>

bool BridgeMessage::IsDevCommand() const {
    return command == BridgeCommand::EnableDevMode ||
           command == BridgeCommand::DisableDevMode ||
           command == BridgeCommand::SkipToPage ||
           command == BridgeCommand::ExportState ||
           command == BridgeCommand::ImportState ||
           command == BridgeCommand::GetLogs;
}

bool BridgeMessage::IsNavigationCommand() const {
    return command == BridgeCommand::NavigateTo ||
           command == BridgeCommand::GoBack ||
           command == BridgeCommand::GoNext ||
           command == BridgeCommand::Cancel;
}

bool BridgeMessage::IsDataCommand() const {
    return command >= BridgeCommand::SetRegion &&
           command <= BridgeCommand::SetCustomization;
}

bool BridgeMessage::IsSystemCommand() const {
    return command == BridgeCommand::GetState ||
           command == BridgeCommand::SetState ||
           command == BridgeCommand::GetLogs;
}

MessageHandler::MessageHandler() {
    Logger::Instance().Debug("MessageHandler created");
}

void MessageHandler::HandleMessage(const std::string& messageJson,
                                    ICoreWebView2* webview) {
    BridgeMessage msg = ParseMessage(messageJson);

    Logger::Instance().Debug("Handling command: " +
        std::to_string(static_cast<int>(msg.command)));

    std::string response;
    auto it = m_handlers.find(msg.command);
    if (it != m_handlers.end()) {
        try {
            response = it->second(msg);
        } catch (const std::exception& e) {
            Logger::Instance().Error(std::string("Handler exception: ") + e.what());
            response = CreateError(msg.requestId, e.what());
        }
    } else if (m_defaultHandler) {
        response = m_defaultHandler(msg);
    } else {
        response = CreateError(msg.requestId, "Unhandled command");
    }

    if (!response.empty() && webview) {
        webview->PostWebMessageAsString(
            std::wstring(response.begin(), response.end()).c_str());
    }
}

std::string MessageHandler::CallHandler(BridgeCommand command, const BridgeMessage& msg) {
    auto it = m_handlers.find(command);
    if (it != m_handlers.end()) {
        try {
            return it->second(msg);
        } catch (const std::exception& e) {
            return CreateError(msg.requestId, e.what());
        }
    }
    if (m_defaultHandler) {
        return m_defaultHandler(msg);
    }
    return CreateError(msg.requestId, "Unhandled command");
}

void MessageHandler::RegisterHandler(BridgeCommand command, HandlerCallback handler) {
    m_handlers[command] = std::move(handler);
}

void MessageHandler::SetDefaultHandler(HandlerCallback handler) {
    m_defaultHandler = std::move(handler);
}

BridgeMessage MessageHandler::ParseMessage(const std::string& json) {
    BridgeMessage msg;

    auto findValue = [&](const std::string& key) -> std::string {
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return {};

        auto colon = json.find(':', pos);
        if (colon == std::string::npos) return {};

        auto start = json.find('"', colon + 1);
        if (start == std::string::npos) return {};

        auto end = json.find('"', start + 1);
        if (end == std::string::npos) return {};

        return json.substr(start + 1, end - start - 1);
    };

    std::string cmd = findValue("command");
    msg.payload = findValue("payload");
    msg.requestId = findValue("requestId");

    if (cmd == "navigateTo") msg.command = BridgeCommand::NavigateTo;
    else if (cmd == "goBack") msg.command = BridgeCommand::GoBack;
    else if (cmd == "goNext") msg.command = BridgeCommand::GoNext;
    else if (cmd == "cancel") msg.command = BridgeCommand::Cancel;
    else if (cmd == "reset") msg.command = BridgeCommand::Reset;
    else if (cmd == "setRegion") msg.command = BridgeCommand::SetRegion;
    else if (cmd == "setKeyboardLayout") msg.command = BridgeCommand::SetKeyboardLayout;
    else if (cmd == "setKeyboardLayoutSecondary") msg.command = BridgeCommand::SetKeyboardLayoutSecondary;
    else if (cmd == "setNetworkConfig") msg.command = BridgeCommand::SetNetworkConfig;
    else if (cmd == "acceptLicense") msg.command = BridgeCommand::AcceptLicense;
    else if (cmd == "setDeviceName") msg.command = BridgeCommand::SetDeviceName;
    else if (cmd == "setAccountInfo") msg.command = BridgeCommand::SetAccountInfo;
    else if (cmd == "setPassword") msg.command = BridgeCommand::SetPassword;
    else if (cmd == "setPrivacySettings") msg.command = BridgeCommand::SetPrivacySettings;
    else if (cmd == "setCustomization") msg.command = BridgeCommand::SetCustomization;
    else if (cmd == "submitAll") msg.command = BridgeCommand::SubmitAll;
    else if (cmd == "getState") msg.command = BridgeCommand::GetState;
    else if (cmd == "setState") msg.command = BridgeCommand::SetState;
    else if (cmd == "exportState") msg.command = BridgeCommand::ExportState;
    else if (cmd == "importState") msg.command = BridgeCommand::ImportState;
    else if (cmd == "getLogs") msg.command = BridgeCommand::GetLogs;
    else if (cmd == "enableDevMode") msg.command = BridgeCommand::EnableDevMode;
    else if (cmd == "disableDevMode") msg.command = BridgeCommand::DisableDevMode;
    else if (cmd == "skipToPage") msg.command = BridgeCommand::SkipToPage;
    else if (cmd == "getWallpaper") msg.command = BridgeCommand::GetWallpaper;
    else if (cmd == "notify") msg.command = BridgeCommand::Notify;
    else msg.command = BridgeCommand::Unknown;

    return msg;
}

std::string MessageHandler::CreateResponse(const std::string& requestId,
                                            bool success,
                                            const std::string& data) {
    std::stringstream ss;
    ss << "{"
       << "\"requestId\":\"" << JsonUtils::EscapeString(requestId) << "\","
       << "\"success\":" << (success ? "true" : "false") << ","
       << "\"data\":";

    if (data.empty()) {
        ss << "{}";
    } else if (data[0] == '{' || data[0] == '[') {
        ss << data;
    } else {
        ss << "\"" << JsonUtils::EscapeString(data) << "\"";
    }

    ss << "}";
    return ss.str();
}

std::string MessageHandler::CreateError(const std::string& requestId,
                                         const std::string& error) {
    return CreateResponse(requestId, false, "error: " + error);
}
