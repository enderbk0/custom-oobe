#pragma once

#include <windows.h>
#include <string>
#include <memory>

class WebViewHost;
class BackendService;

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    bool Create();
    void Show(int nCmdShow = SW_SHOWNORMAL);
    void CenterOnScreen();
    HWND GetHandle() const { return m_hWnd; }

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg,
                                        WPARAM wParam, LPARAM lParam);

    void OnResize(int width, int height);
    void OnDpiChange(WPARAM wParam, LPARAM lParam);
    void OnClose();

private:
    bool RegisterWindowClass();
    bool InitializeComponents();

    HWND m_hWnd = nullptr;
    HINSTANCE m_hInstance = nullptr;

    std::unique_ptr<WebViewHost> m_webviewHost;
    std::shared_ptr<BackendService> m_backend;

    int m_width = 1366;
    int m_height = 768;
    int m_minWidth = 1280;
    int m_minHeight = 720;
    UINT m_dpi = 96;

    static constexpr wchar_t CLASS_NAME[] = L"CustomOOBEWindow";
    static constexpr wchar_t WINDOW_TITLE[] = L"Windows Setup";
};
