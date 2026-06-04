#pragma once

#include <windows.h>
#include <memory>

class MainWindow;

class App {
public:
    App();
    ~App();

    bool Initialize(HINSTANCE hInstance);
    int Run();
    void Shutdown();

    HINSTANCE GetInstance() const { return m_hInstance; }

private:
    HINSTANCE m_hInstance = nullptr;
    bool m_initialized = false;
    std::unique_ptr<MainWindow> m_mainWindow;
};
