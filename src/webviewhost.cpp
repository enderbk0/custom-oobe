#include "webviewhost.h"
#include "backend.h"
#include "bridgemessage.h"
#include "config.h"
#include "logging.h"
#include <wrl.h>

using namespace Microsoft::WRL;

WebViewHost::WebViewHost() {
    m_frontendPath = ConfigManager::Instance().GetConfig().frontendPath;
    Logger::Instance().Debug("WebViewHost created, frontend path: " +
        std::string(m_frontendPath.begin(), m_frontendPath.end()));
}

WebViewHost::~WebViewHost() {
    if (m_controller) {
        m_controller->Close();
    }
    Logger::Instance().Debug("WebViewHost destroyed");
}

bool WebViewHost::Initialize(HWND parentWindow) {
    m_parentWindow = parentWindow;
    Logger::Instance().Info("WebViewHost initializing");

    HRESULT hr = CreateEnvironmentWithRetry(parentWindow,
        m_controller.put(), m_webview.put());

    if (FAILED(hr)) {
        Logger::Instance().Error("Failed to create WebView2 environment: 0x" +
            std::to_string(hr));
        return false;
    }

    if (!m_webview) {
        Logger::Instance().Error("WebView2 creation returned null");
        return false;
    }

    hr = m_webview->get_Settings(&m_settings);
    if (SUCCEEDED(hr) && m_settings) {
        m_settings->put_AreDefaultScriptDialogsEnabled(FALSE);
        m_settings->put_IsScriptEnabled(TRUE);
        m_settings->put_IsWebMessageEnabled(TRUE);
        m_settings->put_AreDefaultScriptDialogsEnabled(FALSE);
        m_settings->put_IsStatusBarEnabled(FALSE);
    }

    RECT bounds;
    GetClientRect(parentWindow, &bounds);
    m_controller->put_Bounds(bounds);

    SetupEventHandlers(m_webview.get());

    std::wstring indexPath = m_frontendPath + L"\\index.html";
    Logger::Instance().Info("Navigating to: " +
        std::string(indexPath.begin(), indexPath.end()));
    m_webview->Navigate(indexPath.c_str());

    m_initialized = true;
    Logger::Instance().Info("WebViewHost initialized successfully");
    return true;
}

void WebViewHost::Resize(int width, int height) {
    if (m_controller) {
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

HRESULT WebViewHost::CreateEnvironmentWithRetry(HWND hWnd,
    ICoreWebView2Controller** controller,
    ICoreWebView2** webview) {

    wil::com_ptr<ICoreWebView2Environment> env;
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&env, hWnd, controller, webview](HRESULT result,
                ICoreWebView2Environment* environment) -> HRESULT {
                if (FAILED(result) || !environment) return result;
                env = environment;
                return environment->CreateCoreWebView2Controller(hWnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [controller, webview](HRESULT result,
                            ICoreWebView2Controller* ctrl) -> HRESULT {
                            if (FAILED(result) || !ctrl) return result;
                            *controller = ctrl;
                            (*controller)->AddRef();
                            ctrl->get_CoreWebView2(webview);
                            return S_OK;
                        }).Get());
            }).Get());

    return hr;
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
                    std::string msg(wmsg.begin(), wmsg.end());
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
                Logger::Instance().Info("WebView2 navigation completed");
                return S_OK;
            }).Get(), token.get());
}
