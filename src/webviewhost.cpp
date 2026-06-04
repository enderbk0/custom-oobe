#include "webviewhost.h"
#include "backend.h"
#include "bridgemessage.h"
#include "config.h"
#include "logging.h"
#include <WebView2.h>
#include <wrl.h>
#include <algorithm>

using namespace Microsoft::WRL;

static std::string NarrowString(const std::wstring& wide) {
    if (wide.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return {};
    std::string result(static_cast<size_t>(len) - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, result.data(), len, nullptr, nullptr);
    return result;
}

using CreateWebView2EnvironmentFn = HRESULT(WINAPI*)(
    PCWSTR browserExecutableFolder,
    PCWSTR userDataFolder,
    ICoreWebView2EnvironmentOptions* environmentOptions,
    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* environmentCreatedHandler);

static CreateWebView2EnvironmentFn LoadWebView2Loader() {
    HMODULE mod = LoadLibraryW(L"WebView2Loader.dll");
    if (!mod) return nullptr;
    auto fn = reinterpret_cast<CreateWebView2EnvironmentFn>(
        GetProcAddress(mod, "CreateCoreWebView2EnvironmentWithOptions"));
    if (!fn) {
        FreeLibrary(mod);
        return nullptr;
    }
    return fn;
}

WebViewHost::WebViewHost() {
    m_frontendPath = ConfigManager::Instance().GetConfig().frontendPath;
    Logger::Instance().Debug("WebViewHost created, frontend path: " +
        NarrowString(m_frontendPath));
}

WebViewHost::~WebViewHost() {
    if (m_controller) {
        m_controller->Close();
    }
    Logger::Instance().Debug("WebViewHost destroyed");
}

bool WebViewHost::Initialize(HWND parentWindow) {
    m_parentWindow = parentWindow;
    Logger::Instance().Info("WebViewHost initializing (dynamic load)");

    auto createEnvFn = LoadWebView2Loader();
    if (!createEnvFn) {
        Logger::Instance().Warning(
            "WebView2Loader.dll not found. "
            "Install WebView2 Runtime from https://go.microsoft.com/fwlink/p/?LinkId=2124703");
        return false;
    }

    HRESULT hr = createEnvFn(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result,
                ICoreWebView2Environment* environment) -> HRESULT {
                if (FAILED(result)) {
                    Logger::Instance().Error(
                        "WebView2 environment creation failed: 0x" +
                        std::to_string(result));
                    return result;
                }
                if (!environment) {
                    Logger::Instance().Error(
                        "WebView2 environment is null");
                    return E_FAIL;
                }
                return environment->CreateCoreWebView2Controller(
                    m_parentWindow,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result,
                            ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) {
                                Logger::Instance().Error(
                                    "WebView2 controller creation failed: 0x" +
                                    std::to_string(result));
                                return result;
                            }
                            if (!controller) {
                                Logger::Instance().Error(
                                    "WebView2 controller is null");
                                return E_FAIL;
                            }

                            m_controller = controller;
                            m_controller->get_CoreWebView2(&m_webview);

                            if (!m_webview) {
                                Logger::Instance().Error(
                                    "Failed to get CoreWebView2 from controller");
                                return E_FAIL;
                            }

                            m_webview->get_Settings(&m_settings);
                            if (m_settings) {
                                m_settings->put_IsScriptEnabled(TRUE);
                                m_settings->put_IsWebMessageEnabled(TRUE);
                                m_settings->put_AreDefaultScriptDialogsEnabled(FALSE);
                                m_settings->put_IsStatusBarEnabled(FALSE);
                            }

                            RECT bounds;
                            GetClientRect(m_parentWindow, &bounds);
                            m_controller->put_Bounds(bounds);

                            SetupEventHandlers(m_webview.get());

                            std::wstring indexPath =
                                m_frontendPath + L"\\index.html";
                            std::wstring fileUrl = L"file:///" + indexPath;
                            std::replace(fileUrl.begin(), fileUrl.end(), L'\\', L'/');
                            Logger::Instance().Info(
                                "Navigating to: " + NarrowString(fileUrl));
                            MessageBoxW(nullptr, fileUrl.c_str(), L"WebView2 Navigate To", MB_OK);
                            m_webview->Navigate(fileUrl.c_str());

                            m_initialized = true;
                            Logger::Instance().Info(
                                "WebView2 initialized successfully");
                            return S_OK;
                        }).Get());
            }).Get());

    if (FAILED(hr)) {
        Logger::Instance().Error(
            "Failed to initiate WebView2 creation: 0x" +
            std::to_string(hr));
        return false;
    }

    return true;
}

void WebViewHost::Resize(int width, int height) {
    if (m_initialized && m_controller) {
        RECT bounds = {0, 0, width, height};
        m_controller->put_Bounds(bounds);
    }
}

void WebViewHost::Navigate(const std::wstring& url) {
    if (m_webview) {
        m_webview->Navigate(url.c_str());
    }
}

void WebViewHost::PostMessage(const std::wstring& message) {
    if (m_webview) {
        m_webview->PostWebMessageAsString(message.c_str());
    }
}

void WebViewHost::SetBackend(std::shared_ptr<BackendService> backend) {
    m_backend = backend;
    m_handler = backend ? backend->GetHandler() : nullptr;
}

void WebViewHost::SetupEventHandlers(ICoreWebView2* webview) {
    auto token = std::make_shared<EventRegistrationToken>();

    webview->add_WebMessageReceived(
        Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [this](ICoreWebView2* sender,
                ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                wil::unique_cotaskmem_string messageRaw;
                args->TryGetWebMessageAsString(&messageRaw);
                if (messageRaw && m_handler) {
                    std::wstring wmsg(messageRaw.get());
                    std::string msg = NarrowString(wmsg);
                    Logger::Instance().Debug("Message from web: " + msg);
                    m_handler->HandleMessage(msg, sender);
                }
                return S_OK;
            }).Get(), token.get());

    webview->add_NavigationStarting(
        Callback<ICoreWebView2NavigationStartingEventHandler>(
            [](ICoreWebView2* sender,
                ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
                return S_OK;
            }).Get(), token.get());

    webview->add_NavigationCompleted(
        Callback<ICoreWebView2NavigationCompletedEventHandler>(
            [this](ICoreWebView2* sender,
                ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                BOOL success = FALSE;
                COREWEBVIEW2_WEB_ERROR_STATUS errStatus;
                args->get_IsSuccess(&success);
                args->get_WebErrorStatus(&errStatus);
                Logger::Instance().Info("WebView2 nav completed, success=" +
                    std::to_string(success) + ", error=" +
                    std::to_string(static_cast<int>(errStatus)));
                return S_OK;
            }).Get(), token.get());
}
