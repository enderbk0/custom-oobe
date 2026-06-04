#pragma once

#include <windows.h>
#include <wil/com.h>
#include <WebView2.h>
#include <string>
#include <memory>

class BackendService;
class MessageHandler;

class WebViewHost {
public:
    WebViewHost();
    ~WebViewHost();

    bool Initialize(HWND parentWindow);
    void Resize(int width, int height);
    void Navigate(const std::wstring& url);
    void PostMessage(const std::wstring& message);
    void SetBackend(std::shared_ptr<BackendService> backend);

    bool IsInitialized() const { return m_initialized; }

private:
    void SetupEventHandlers(ICoreWebView2* webview);

    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webview;
    wil::com_ptr<ICoreWebView2Settings> m_settings;

    HWND m_parentWindow = nullptr;
    std::shared_ptr<BackendService> m_backend;
    std::shared_ptr<MessageHandler> m_handler;

    bool m_initialized = false;
    std::wstring m_frontendPath;
};
