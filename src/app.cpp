#include "app.h"
#include "mainwindow.h"
#include "config.h"
#include "logging.h"
#include "statemanager.h"
#include <shellapi.h>

App::App() {
}

App::~App() {
    Shutdown();
}

bool App::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    MessageBoxW(nullptr, L"Step 1: ConfigManager init", L"Debug", MB_OK);
    if (!ConfigManager::Instance().Initialize()) {
        MessageBoxW(nullptr, L"Failed to initialize configuration.",
                     L"Custom OOBE", MB_ICONERROR);
        return false;
    }
    MessageBoxW(nullptr, L"Step 2: ConfigManager done", L"Debug", MB_OK);

    Logger::Instance().Initialize(ConfigManager::Instance().GetConfig().logPath);
    MessageBoxW(nullptr, L"Step 3: Logger done", L"Debug", MB_OK);
    StateManager::Instance().Initialize(ConfigManager::Instance().GetConfig().statePath);
    MessageBoxW(nullptr, L"Step 4: StateManager done", L"Debug", MB_OK);
    Logger::Instance().Info("=== Custom OOBE Starting ===");
    Logger::Instance().Info("Version: 1.0.0");
    MessageBoxW(nullptr, L"Step 5: Log writes done", L"Debug", MB_OK);

    m_mainWindow = std::make_unique<MainWindow>();
    MessageBoxW(nullptr, L"Step 6: MainWindow constructed", L"Debug", MB_OK);
    if (!m_mainWindow->Create()) {
        Logger::Instance().Error("Failed to create main window");
        MessageBoxW(nullptr, L"Step 7a: MainWindow::Create FAILED", L"Debug", MB_OK);
        return false;
    }
    MessageBoxW(nullptr, L"Step 7: MainWindow::Create done", L"Debug", MB_OK);

    m_mainWindow->Show();
    MessageBoxW(nullptr, L"Step 8: Show done", L"Debug", MB_OK);
    m_mainWindow->CenterOnScreen();
    MessageBoxW(nullptr, L"Step 9: Center done", L"Debug", MB_OK);

    m_initialized = true;
    Logger::Instance().Info("App initialized successfully");
    MessageBoxW(nullptr, L"Step 10: Initialize returning true", L"Debug", MB_OK);
    return true;
}

int App::Run() {
    if (!m_initialized) {
        Logger::Instance().Error("App not initialized");
        return 1;
    }

    Logger::Instance().Info("Entering message loop");
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Logger::Instance().Info("Message loop exited");
    return static_cast<int>(msg.wParam);
}

void App::Shutdown() {
    if (m_initialized) {
        Logger::Instance().Info("=== Custom OOBE Shutting Down ===");
        m_initialized = false;
    }
}
