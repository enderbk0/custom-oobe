#include "app.h"
#include "logging.h"
#include <commctrl.h>
#include <shellscalingapi.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shcore.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

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
        return 1;
    }

    int result = app.Run();
    app.Shutdown();

    return result;
}
