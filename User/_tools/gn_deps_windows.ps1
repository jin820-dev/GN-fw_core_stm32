# ==============================================================
# GN Firmware Third Party Setup Script (Windows / PowerShell)
# - clone afiskon/stm32-ssd1306
# - copy required files to User/third_party/afiskon_ssd1306
# - patch ssd1306_conf.h to enable STM32H7 only
# ==============================================================

$ErrorActionPreference = "Stop"

$repoUrl  = "https://github.com/afiskon/stm32-ssd1306.git"
$tmpDir   = "_tmp_ssd1306"
$dstPath  = "User/third_party/afiskon_ssd1306"

$requiredFiles = @(
    "ssd1306.c",
    "ssd1306.h",
    "ssd1306_fonts.c",
    "ssd1306_fonts.h",
    "ssd1306_conf.h",
    "LICENSE",
    "README.md"
)

function Require-Command($cmd) {
    & $cmd --version > $null 2>&1
    if ($LASTEXITCODE -ne 0) { throw "$cmd is not installed. Please install it first." }
}

function Find-RepoFile($root, $fileName) {
    # repo内を再帰検索して、最初に見つかったパスを返す
    $hit = Get-ChildItem -Path $root -Recurse -File -Filter $fileName -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($null -eq $hit) { return $null }
    return $hit.FullName
}

Write-Host "Checking git..." -ForegroundColor Cyan
Require-Command git

Write-Host "Cleaning old temp..." -ForegroundColor Cyan
if (Test-Path $tmpDir)  { Remove-Item $tmpDir -Recurse -Force }
if (Test-Path $dstPath) { Remove-Item $dstPath -Recurse -Force }

Write-Host "Cloning SSD1306 repo..." -ForegroundColor Cyan
git clone --depth 1 $repoUrl $tmpDir

Write-Host "Copying required files..." -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path $dstPath | Out-Null

foreach ($f in $requiredFiles) {
    $src = Find-RepoFile -root $tmpDir -fileName $f
    if ($null -eq $src) {
        throw "Required file not found in repo: $f (repo structure may have changed)"
    }
    Copy-Item $src $dstPath -Force
    Write-Host "  + $f  <=  $src"
}

Write-Host "Patching ssd1306_conf.h for STM32H7..." -ForegroundColor Cyan
$confPath = Join-Path $dstPath "ssd1306_conf.h"
if (-not (Test-Path $confPath)) { throw "ssd1306_conf.h not found after copy!" }

$lines = Get-Content $confPath

$patched = $lines | ForEach-Object {
    if ($_ -match '^\s*(//\s*)?#\s*define\s+STM32[A-Z0-9_]+') {
        if ($_ -match 'STM32H7') {
            return '#define STM32H7'
        } else {
            return '//#define ' + ($_ -replace '^\s*(//\s*)?#\s*define\s+','')
        }
    }
    return $_
}

Set-Content -Path $confPath -Value $patched -Encoding UTF8

Remove-Item $tmpDir -Recurse -Force

Write-Host ""
Write-Host "=========================================" -ForegroundColor Green
Write-Host " SSD1306 setup complete!" -ForegroundColor Green
Write-Host " You can build immediately." -ForegroundColor Green
Write-Host "=========================================" -ForegroundColor Green
