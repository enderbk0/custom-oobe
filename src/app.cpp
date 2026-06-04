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

    if (!ConfigManager::Instance().Initialize()) {
        MessageBoxW(nullptr, L"Failed to initialize configuration.",
                     L"Custom OOBE", MB_ICONERROR);
        return false;
    }

    Logger::Instance().Initialize(ConfigManager::Instance().GetConfig().logPath);
    StateManager::Instance().Initialize(ConfigManager::Instance().GetConfig().statePath);
    Logger::Instance().Info("=== Custom OOBE Starting ===");
    Logger::Instance().Info("Version: 1.0.0");

    auto mainWindow = std::make_unique<MainWindow>();
    if (!mainWindow->Create()) {
        Logger::Instance().Error("Failed to create main window");
        return false;
    }

    mainWindow->Show();
    mainWindow->CenterOnScreen();

    m_initialized = true;
    Logger::Instance().Info("App initialized successfully");
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
