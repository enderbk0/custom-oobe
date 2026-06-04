#include "app.h"
#include "logging.h"
#include <commctrl.h>
#include <shellscalingapi.h>
#include <objbase.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "ole32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to initialize COM.", L"Error", MB_ICONERROR);
        return 1;
    }

    auto setDpiCtx = reinterpret_cast<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"),
                       "SetProcessDpiAwarenessContext"));
    if (setDpiCtx) {
        setDpiCtx(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }

    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    App app;
    if (!app.Initialize(hInstance)) {
        MessageBoxW(nullptr, L"App::Initialize returned false", L"Debug", MB_OK);
        CoUninitialize();
        return 1;
    }

    int result = app.Run();
    app.Shutdown();
    CoUninitialize();

    return result;
}
