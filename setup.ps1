param(
    [string]$VcpkgDir = "$env:LOCALAPPDATA\vcpkg",
    [switch]$SkipClone
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "=== Custom OOBE Setup ===" -ForegroundColor Cyan
Write-Host ""

# 1. Install vcpkg if needed
$vcpkgExe = Join-Path $VcpkgDir "vcpkg.exe"
if (-not (Test-Path $vcpkgExe)) {
    if (-not (Test-Path $VcpkgDir)) {
        Write-Host "Cloning vcpkg to $VcpkgDir..." -ForegroundColor Yellow
        git clone https://github.com/microsoft/vcpkg $VcpkgDir
        if (-not $?) { throw "Failed to clone vcpkg" }
    }
    Write-Host "Bootstrapping vcpkg..." -ForegroundColor Yellow
    & (Join-Path $VcpkgDir "bootstrap-vcpkg.bat")
    if (-not $?) { throw "Failed to bootstrap vcpkg" }
} else {
    Write-Host "vcpkg already installed at $VcpkgDir" -ForegroundColor Green
}

# 2. Install dependencies
Write-Host "`nInstalling dependencies..." -ForegroundColor Yellow
& $vcpkgExe install --triplet x64-windows
if (-not $?) { throw "Failed to install vcpkg dependencies" }

# 3. Configure CMake
Write-Host "`nConfiguring CMake..." -ForegroundColor Yellow
$toolchain = Join-Path $VcpkgDir "scripts\buildsystems\vcpkg.cmake"
cmake -B "$ProjectRoot\build" -S "$ProjectRoot" `
    -DCMAKE_TOOLCHAIN_FILE="$toolchain" `
    -DVCPKG_TARGET_TRIPLET=x64-windows `
    -DCMAKE_BUILD_TYPE=Release
if (-not $?) { throw "CMake configure failed" }

# 4. Build
Write-Host "`nBuilding..." -ForegroundColor Yellow
cmake --build "$ProjectRoot\build" --config Release
if (-not $?) { throw "Build failed" }

Write-Host "`n=== Setup complete ===" -ForegroundColor Cyan
Write-Host "Executable: $ProjectRoot\build\Release\custom-oobe.exe"
