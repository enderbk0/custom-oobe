param(
    [string]$OutputDir = ".",
    [string]$Arch = "x64"
)

$ErrorActionPreference = "Stop"

$extractDir = Join-Path $OutputDir "WebView2Runtime"

Write-Host "=== Download WebView2 Fixed Version Runtime ($Arch) ===" -ForegroundColor Cyan

if (Test-Path $extractDir) {
    Write-Host "Runtime already exists at $extractDir" -ForegroundColor Green
    exit 0
}

Write-Host "Fetching latest release info from westinyang/WebView2RuntimeArchive..." -ForegroundColor Yellow
$release = Invoke-RestMethod -Uri "https://api.github.com/repos/westinyang/WebView2RuntimeArchive/releases/latest"
$tag = $release.tag_name
Write-Host "Latest version: $tag" -ForegroundColor Green

$assetName = "Microsoft.WebView2.FixedVersionRuntime.$tag.$Arch.cab"
$asset = $release.assets | Where-Object { $_.name -eq $assetName }
if (-not $asset) {
    Write-Host "Asset '$assetName' not found in latest release. Available assets:" -ForegroundColor Red
    $release.assets.name | ForEach-Object { Write-Host "  $_" }
    exit 1
}

$cabUrl = $asset.browser_download_url
$cabPath = "$env:TEMP\wv2_runtime.cab"

Write-Host "Downloading from: $cabUrl" -ForegroundColor Yellow
try {
    $wc = New-Object System.Net.WebClient
    $wc.DownloadFile($cabUrl, $cabPath)
} catch {
    Write-Host "Download failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host "Extracting to $extractDir..." -ForegroundColor Yellow
try {
    $tmpDir = "$env:TEMP\wv2_runtime_extract"
    if (Test-Path $tmpDir) { Remove-Item $tmpDir -Recurse -Force }
    New-Item -ItemType Directory -Path $tmpDir -Force | Out-Null

    & expand $cabPath -F:* $tmpDir
    if (-not $?) { throw "expand failed" }

    $innerDir = Get-ChildItem $tmpDir -Directory | Select-Object -First 1
    if (-not $innerDir) { throw "No subfolder found in extracted CAB" }

    New-Item -ItemType Directory -Path $extractDir -Force | Out-Null
    Move-Item "$($innerDir.FullName)\*" $extractDir -Force
    Remove-Item $tmpDir -Recurse -Force
    Remove-Item $cabPath -Force -ErrorAction SilentlyContinue
} catch {
    Write-Host "Extraction failed: $_" -ForegroundColor Red
    Remove-Item $cabPath -Force -ErrorAction SilentlyContinue
    Remove-Item "$env:TEMP\wv2_runtime_extract" -Recurse -Force -ErrorAction SilentlyContinue
    exit 1
}

Write-Host "Done. Runtime extracted to: $extractDir" -ForegroundColor Green
Write-Host "Size: $(Get-ChildItem $extractDir -Recurse | Measure-Object Length -Sum | ForEach-Object { '{0:N0} KB' -f ($_.Sum / 1KB) } )"
