param(
    [string]$OutputDir = ".",
    [string]$Arch = "x64"
)

$ErrorActionPreference = "Stop"

$runtimeUrl = "https://msedgewebview2assets.blob.core.windows.net/runtime/webview2_runtime_$($Arch)_fixed_version.zip"
$zipPath = "$env:TEMP\webview2_runtime.zip"
$extractDir = Join-Path $OutputDir "WebView2Runtime"

Write-Host "=== Download WebView2 Fixed Version Runtime ($Arch) ===" -ForegroundColor Cyan

if (Test-Path $extractDir) {
    Write-Host "Runtime already exists at $extractDir" -ForegroundColor Green
    exit 0
}

Write-Host "Downloading from: $runtimeUrl" -ForegroundColor Yellow
try {
    $wc = New-Object System.Net.WebClient
    $wc.DownloadFile($runtimeUrl, $zipPath)
} catch {
    Write-Host "Download failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host "Extracting to $extractDir..." -ForegroundColor Yellow
try {
    Expand-Archive -Path $zipPath -DestinationPath $extractDir -Force
} catch {
    Write-Host "Extraction failed: $_" -ForegroundColor Red
    Remove-Item $zipPath -Force -ErrorAction SilentlyContinue
    exit 1
}

Remove-Item $zipPath -Force -ErrorAction SilentlyContinue
Write-Host "Done. Runtime extracted to: $extractDir" -ForegroundColor Green
Write-Host "Size: $(Get-ChildItem $extractDir -Recurse | Measure-Object Length -Sum | ForEach-Object { '{0:N0} KB' -f ($_.Sum / 1KB) } )"
