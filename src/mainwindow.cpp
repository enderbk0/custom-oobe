#include "mainwindow.h"
#include "resource.h"
#include "webviewhost.h"
#include "backend.h"
#include "config.h"
#include "logging.h"
#include <shellscalingapi.h>

#pragma comment(lib, "Shcore.lib")

MainWindow::MainWindow() {
    m_hInstance = GetModuleHandleW(nullptr);
    Logger::Instance().Debug("MainWindow created");
}

MainWindow::~MainWindow() {
    if (m_webviewHost) {
        m_webviewHost.reset();
    }
    if (m_backend) {
        m_backend->Shutdown();
    }
    Logger::Instance().Debug("MainWindow destroyed");
}

bool MainWindow::Create() {
    if (!RegisterWindowClass()) {
        Logger::Instance().Error("Failed to register window class");
        return false;
    }

    m_dpi = GetDpiForSystem();

    const auto& config = ConfigManager::Instance().GetConfig();
    m_width = config.windowWidth;
    m_height = config.windowHeight;
    m_minWidth = config.minWidth;
    m_minHeight = config.minHeight;

    DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    DWORD exStyle = WS_EX_ACCEPTFILES;

    RECT desktopRect;
    GetWindowRect(GetDesktopWindow(), &desktopRect);

    int x = (desktopRect.right - m_width) / 2;
    int y = (desktopRect.bottom - m_height) / 2;

    RECT windowRect = {0, 0, m_width, m_height};
    AdjustWindowRectExForDpi(&windowRect, style, FALSE, exStyle, m_dpi);

    m_hWnd = CreateWindowExW(
        exStyle,
        CLASS_NAME,
        WINDOW_TITLE,
        style,
        x, y,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    if (!m_hWnd) {
        DWORD err = GetLastError();
        Logger::Instance().Error("Failed to create window: " +
            std::to_string(err));
        std::wstring errMsg = L"CreateWindowExW failed with error: " +
            std::to_wstring(err);
        MessageBoxW(nullptr, errMsg.c_str(), L"Error", MB_OK);
        return false;
    }

    Logger::Instance().Info("Window created: " +
        std::to_string(m_width) + "x" + std::to_string(m_height));

    if (!InitializeComponents()) {
        Logger::Instance().Error("Failed to initialize components");
        return false;
    }

    return true;
}

void MainWindow::Show(int nCmdShow) {
    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);
    Logger::Instance().Info("Window displayed");
}

void MainWindow::CenterOnScreen() {
    RECT desktopRect;
    GetWindowRect(GetDesktopWindow(), &desktopRect);

    RECT windowRect;
    GetWindowRect(m_hWnd, &windowRect);

    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;

    int x = (desktopRect.right - width) / 2;
    int y = (desktopRect.bottom - height) / 2;

    SetWindowPos(m_hWnd, nullptr, x, y, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER);
}

bool MainWindow::RegisterWindowClass() {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIconW(m_hInstance, MAKEINTRESOURCEW(IDI_APP_ICON));
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
    wc.hbrBackground = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIconW(m_hInstance, MAKEINTRESOURCEW(IDI_APP_ICON));

    return RegisterClassExW(&wc) != 0;
}

bool MainWindow::InitializeComponents() {
    m_backend = std::make_shared<BackendService>();
    if (!m_backend->Initialize()) {
        Logger::Instance().Error("Backend initialization failed");
        return false;
    }

    m_webviewHost = std::make_unique<WebViewHost>();
    m_webviewHost->SetBackend(m_backend);

    if (!m_webviewHost->Initialize(m_hWnd)) {
        Logger::Instance().Warning("WebView2 initialization failed. "
            "Ensure WebView2 Runtime is installed.");
    }

    Logger::Instance().Info("Components initialized");
    return true;
}

void MainWindow::OnResize(int width, int height) {
    if (m_webviewHost) {
        m_webviewHost->Resize(width, height);
    }
}

void MainWindow::OnDpiChange(WPARAM wParam, LPARAM lParam) {
    UINT newDpi = HIWORD(wParam);
    if (newDpi != m_dpi) {
        m_dpi = newDpi;
        RECT rect;
        GetWindowRect(m_hWnd, &rect);
        SetWindowPos(m_hWnd, nullptr, rect.left, rect.top,
                     rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOZORDER | SWP_FRAMECHANGED);
        Logger::Instance().Debug("DPI changed to: " + std::to_string(m_dpi));
    }
}

void MainWindow::OnClose() {
    Logger::Instance().Info("Window closing");
    if (m_backend) {
        m_backend->Shutdown();
    }
    DestroyWindow(m_hWnd);
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hWnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pThis = static_cast<MainWindow*>(createStruct->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<MainWindow*>(
            GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    if (!pThis) {
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    switch (msg) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            pThis->OnResize(width, height);
            return 0;
        }

        case WM_GETMINMAXINFO: {
            auto* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
            mmi->ptMinTrackSize.x = pThis->m_minWidth;
            mmi->ptMinTrackSize.y = pThis->m_minHeight;
            return 0;
        }

        case WM_DPICHANGED: {
            pThis->OnDpiChange(wParam, lParam);
            auto* rect = reinterpret_cast<RECT*>(lParam);
            SetWindowPos(hWnd, nullptr,
                         rect->left, rect->top,
                         rect->right - rect->left,
                         rect->bottom - rect->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            return 0;
        }

        case WM_CLOSE: {
            pThis->OnClose();
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SETFOCUS: {
            return 0;
        }
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
