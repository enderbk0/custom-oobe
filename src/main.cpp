#include "app.h"
#include "logging.h"
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    App app;
    if (!app.Initialize(hInstance)) {
        return 1;
    }

    int result = app.Run();
    app.Shutdown();

    return result;
}
