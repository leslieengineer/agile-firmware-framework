param (
    [switch]$Clean,
    [switch]$Test,
    [switch]$App
)

$ProjectRoot = Resolve-Path "$PSScriptRoot\..\.."
Set-Location $ProjectRoot

$BuildDir = "build"

if ($Clean) {
    Write-Host ">>> [1/4] Cleaning build directory..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
}

Write-Host ">>> [2/4] Configuring CMake..." -ForegroundColor Cyan
cmake -S . -B $BuildDir
if ($LASTEXITCODE -ne 0) { 
    Write-Host "[ERROR] CMake Config Failed!" -ForegroundColor Red
    exit $LASTEXITCODE 
}

Write-Host ">>> [3/4] Building project..." -ForegroundColor Cyan
cmake --build $BuildDir
if ($LASTEXITCODE -ne 0) { 
    Write-Host "[ERROR] Build Failed!" -ForegroundColor Red
    exit $LASTEXITCODE 
}
Write-Host "[SUCCESS] Build Successful!" -ForegroundColor Green

Write-Host ">>> [4/4] Execution Phase" -ForegroundColor Cyan

if ($Test) {
    Write-Host ">>> Running Tests..." -ForegroundColor Magenta
    ctest --test-dir $BuildDir --output-on-failure
}

if ($App) {
    Write-Host ">>> Running App..." -ForegroundColor Magenta
    $AppPath = ".\$BuildDir\products\env_monitor\env_monitor.exe"
    if (Test-Path $AppPath) {
        & $AppPath
    } else {
        Write-Host "[ERROR] Cannot find executable!" -ForegroundColor Red
    }
}

if (-not $Test -and -not $App) {
    Write-Host "No execution flags provided." -ForegroundColor DarkGray
    Write-Host "Hint: Use .\tools\scripts\build.ps1 -Test or -App" -ForegroundColor DarkGray
}