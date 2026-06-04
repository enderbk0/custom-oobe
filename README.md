# Custom Windows OOBE Experience

A production-quality, standalone Windows application that mimics the Windows 11 Out-of-Box Experience (OOBE) workflow. Built with C++/Win32 and WebView2 — zero .NET dependencies.

## Architecture

```
┌─────────────────────────────────────────────┐
│              C++ Host (Win32)               │
│  ┌─────────┐ ┌──────────┐ ┌──────────────┐ │
│  │  WinMain │ │ MainWindow│ │ WebViewHost  │ │
│  └────┬────┘ └────┬─────┘ └──────┬───────┘ │
│       │           │              │          │
│  ┌────▼───────────▼──────────────▼───────┐  │
│  │          BackendService              │  │
│  │  ┌────────┐ ┌──────────┐ ┌────────┐  │  │
│  │  │ Config │ │StateMgmt │ │ Logger │  │  │
│  │  └────────┘ └──────────┘ └────────┘  │  │
│  └──────────────────────────────────────┘  │
└──────────────────┬──────────────────────────┘
                   │ postMessage / WebMessageReceived
┌──────────────────▼──────────────────────────┐
│           WebView2 (Edge Chromium)          │
│  ┌──────────────────────────────────────┐  │
│  │         Frontend (HTML/CSS/JS)      │  │
│  │  ┌──────┐ ┌────────┐ ┌──────────┐  │  │
│  │  │Router│ │  State │ │ Bridge   │  │  │
│  │  ├──────┤ ├────────┤ ├──────────┤  │  │
│  │  │Pages │ │Validat.│ │ DevMode  │  │  │
│  │  └──────┘ └────────┘ └──────────┘  │  │
│  └──────────────────────────────────────┘  │
└─────────────────────────────────────────────┘
```

### Layers

| Layer | Technology | Responsibility |
|-------|-----------|----------------|
| **Host** | C++ / Win32 API | Window management, DPI handling, message loop |
| **WebView** | WebView2 Win32 C++ | HTML/CSS/JS rendering, host-web bridge |
| **Backend** | C++ | State, config, logging, business logic |
| **Frontend** | HTML/CSS/JS | UI rendering, animations, user interaction |
| **Bridge** | postMessage API | JSON-based host-web communication |

## Project Structure

```
windows-oobe/
├── .github/workflows/build.yml    # CI build pipeline
├── CMakeLists.txt                  # CMake build configuration
├── generate-icon.ps1              # Icon generation script
├── README.md                      # This file
├── src/
│   ├── main.cpp                   # WinMain entry point
│   ├── app.h / app.cpp            # Application lifecycle
│   ├── mainwindow.h / .cpp        # Win32 window + DPI support
│   ├── webviewhost.h / .cpp       # WebView2 hosting + bridge
│   ├── backend.h / .cpp           # Service layer + simulated ops
│   ├── config.h / .cpp            # JSON config management
│   ├── statemanager.h / .cpp      # State persistence (JSON)
│   ├── logging.h / .cpp           # File + debug output logging
│   ├── bridgemessage.h / .cpp     # Message parsing/routing
│   └── resource.h                 # Resource identifiers
├── resources/
│   ├── app.rc                     # Windows resource script
│   ├── app.manifest               # DPI + common controls manifest
│   └── app.ico                    # Application icon (32x32)
└── frontend/
    ├── index.html                 # Shell HTML
    ├── css/
    │   ├── themes.css             # Light/dark theme variables
    │   ├── animations.css         # Transition/animation keyframes
    │   ├── fluent.css             # Fluent Design component styles
    │   └── styles.css             # Page layouts and structure
    ├── js/
    │   ├── app.js                 # Main controller
    │   ├── bridge.js              # Host communication layer
    │   ├── state.js               # Client-side state management
    │   ├── router.js              # Page navigation system
    │   ├── validation.js          # Form field validation
    │   ├── devmode.js             # Developer tools panel
    │   └── pages/
    │       ├── welcome.js         # Welcome screen
    │       ├── region.js          # Region selection
    │       ├── keyboard.js        # Primary keyboard layout
    │       ├── keyboard-secondary.js # Secondary keyboard layout
    │       ├── network.js         # Network connection
    │       ├── license.js         # License agreement
    │       ├── device-naming.js   # Device name input
    │       ├── account.js         # Local account creation
    │       ├── password.js        # Password creation
    │       ├── privacy.js         # Privacy settings toggles
    │       ├── customization.js   # Usage customization
    │       ├── summary.js         # Settings review
    │       ├── preparing.js       # Setup progress animation
    │       ├── hi.js              # Welcome screen after setup
    │       └── completion.js      # Setup complete screen
    └── assets/                    # Static assets (future use)
```

## Build Instructions

### Prerequisites

- **Windows 10/11** (x64)
- **Visual Studio 2022** with:
  - Desktop development with C++ workload
  - Windows 11 SDK (10.0.22621.0+)
- **WebView2 Runtime** (included with Edge, or standalone)
- **CMake** 3.20+
- **vcpkg** (for dependency management)

### Using vcpkg (Recommended)

```powershell
# Install dependencies
vcpkg install webview2:x64-windows
vcpkg install wil:x64-windows

# Configure and build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="path\to\vcpkg\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build --config Release
```

### Using Visual Studio

1. Open CMakeLists.txt as a CMake project in Visual Studio 2022
2. Visual Studio will detect and configure automatically
3. Build → Build All (Ctrl+Shift+B)

### Via GitHub Actions

Push to `main` / `master` branch — the CI workflow in `.github/workflows/build.yml` will:
1. Set up VS 2022 BuildTools
2. Install dependencies via vcpkg
3. Build Release configuration
4. Upload artifacts as a ZIP

## Running

Run `build/Release/custom-oobe.exe` — requires WebView2 Runtime.

The application:
- Opens a 1366×768 window centered on screen
- Minimum size: 1280×720
- PerMonitorV2 DPI-aware
- Standard caption buttons
- Resizable

### Navigation

- **Back / Next** buttons in the footer
- **Progress bar** at top (hidden on welcome)
- **Page indicator** ("3 of 15")
- **Enter** submits forms, advances when fields are valid
- **Escape** closes dev panel

### Keyboard Navigation

| Key | Action |
|-----|--------|
| Tab | Move focus forward |
| Shift+Tab | Move focus backward |
| Enter | Activate button / advance |
| Space | Toggle checkbox / select card |
| Ctrl+Shift+D | Toggle Developer Mode |

## Developer Mode

Press **Ctrl+Shift+D** to open the Developer Tools panel:

- **Jump to any page** — skip directly to any screen
- **Reset state** — clear all stored state
- **Export/Import state** — copy/paste JSON state
- **Mock data toggle** — enable simulated data
- **Live log viewer** — see backend logs in real time

## Theme Support

- **Light mode** (default) — white backgrounds, dark text
- **Dark mode** — automatically detected from system preference
- High contrast support via system settings
- CSS custom properties for easy theme extension

## Screen Flow

1. **Welcome** — Introduction screen with "Get started" button
2. **Region** — Select from 35 regions
3. **Keyboard Layout** — Select from 27 keyboard layouts
4. **Secondary Keyboard** — Optionally add a second layout
5. **Network** — Connect or skip network setup
6. **License Agreement** — EULA acceptance with scrollable license
7. **Device Naming** — Enter device name (1-15 chars, alphanumeric + hyphens)
8. **Account Creation** — Enter local account username
9. **Password Creation** — Set password with confirmation and hint
10. **Privacy Settings** — Toggle location, find device, ads, diagnostics, tailored experiences
11. **Optional Customization** — Select device usage pattern
12. **Summary** — Review all selections before proceeding
13. **Preparing Windows** — Animated progress simulation
14. **Hi Screen** — Personalized welcome after setup
15. **Completion** — Final "You're all set!" screen

## Simulated Actions (Phase 1)

All system operations are simulated in Phase 1:

| Action | Simulation |
|--------|-----------|
| User creation | Logged only, no OS account created |
| Password creation | Validated but not stored securely |
| Device naming | Logged only, no hostname change |
| Privacy configuration | Logged only, no registry changes |
| Setup completion | Logged only, no OOBE state changed |

Real implementations should replace `BackendService::Simulate*()` methods.

## Configuration

Stored in `%LOCALAPPDATA%\CustomOOBE\config\settings.json`:

```json
{
  "windowWidth": 1366,
  "windowHeight": 768,
  "minWidth": 1280,
  "minHeight": 720,
  "startMaximized": false,
  "devModeEnabled": false,
  "mockDataEnabled": false
}
```

## Logging

Logs are written to `%LOCALAPPDATA%\CustomOOBE\logs\oobe_YYYYMMDD_HHMMSS.log`

Also visible in:
- Debug output (OutputDebugString)
- Developer Mode log panel (Ctrl+Shift+D)

## Research Sources

### Documented APIs

| API | Source |
|-----|--------|
| `OOBEComplete()` | [Microsoft Learn: oobenotification.h](https://learn.microsoft.com/en-us/windows/win32/api/oobenotification/nf-oobenotification-oobecomplete) |
| `CreateCoreWebView2EnvironmentWithOptions` | [Microsoft Learn: WebView2 Win32](https://learn.microsoft.com/en-us/microsoft-edge/webview2/get-started/win32) |
| `GetDpiForWindow` | [Microsoft Learn: Win32 DPI](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdpiforwindow) |
| `AdjustWindowRectExForDpi` | [Microsoft Learn: Win32 DPI](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-adjustwindowrectexfordpi) |

### Documented Windows Features

| Feature | Source |
|---------|--------|
| OOBE screen flow | [Customize OOBE (Microsoft Learn)](https://learn.microsoft.com/en-us/windows-hardware/customize/desktop/customize-oobe) |
| OOBE screen details | [OOBE Screen Details (Microsoft Learn)](https://learn.microsoft.com/en-us/windows-hardware/customize/desktop/oobe-screen-details) |
| Unattend.xml OOBE settings | [Unattended Windows Setup Reference](https://learn.microsoft.com/en-us/windows-hardware/customize/desktop/unattend) |
| Sysprep | [Sysprep (Microsoft Learn)](https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/sysprep--system-preparation--overview) |

### Third-Party Research

| Topic | Source |
|-------|--------|
| OOBE Complete Timestamp | [patchmypc.com](https://patchmypc.com/blog/oobe-complete-timestamp-hotpatch-delay) |
| OOBE Detection | [oofhours.com](https://oofhours.com/2023/09/15/detecting-when-you-are-in-oobe/) |
| CloudExperienceHost Role | [Windows Forum](https://windowsforum.com/threads/kb5078674-windows-11-oobe-update-setup-ui-and-localization-tweaks.403126/) |

## Known Limitations

1. **Phase 1 only** — This is a desktop prototype, not a real OOBE replacement
2. **No real account creation** — All operations are simulated
3. **No OOBE state modification** — Does not call `OOBEComplete()` or modify registry
4. **No secure desktop** — Runs as a normal user application
5. **No network detection** — Network page is a static mockup
6. **No multilingual support** — UI is English-only (future: resource-based localization)
7. **No accessibility audit** — Basic keyboard nav and ARIA, but no screen reader testing
8. **Single monitor only** — No multi-monitor DPI transition testing
9. **WebView2 Runtime required** — Not embedded; must be pre-installed

## Security Considerations

- **No elevation required** — Runs as `asInvoker`
- **No OS modification** — All simulated actions are sandboxed
- **No network access from host** — WebView2 can navigate to file:// only
- **Content Security Policy** — CSP restricts script/style sources
- **No credential storage** — Passwords are validated but not persisted
- **No telemetry** — No data collection or transmission

## Future Integration Roadmap

### Phase 2 — Desktop Launcher
Integrate `SetupComplete.cmd` or `FirstLogonCommands` to auto-launch custom OOBE after Windows setup completes, before the user reaches the desktop.

**Method:** `unattend.xml` with `FirstLogonCommands` → launch `custom-oobe.exe`

### Phase 3 — Pre-Desktop OOBE
Replace or supplement CloudExperienceHost during Windows setup.

**Method:** Custom WinPE or audit mode boot → custom OOBE → Sysprep generalize

### Risks
- Microsoft does not officially support replacing OOBE
- Windows Updates may overwrite customizations
- Secure boot / TPM requirements may interfere
- Accessibility and language requirements must be met

## License

This project is provided as a reference architecture. No warranty is expressed or implied.
