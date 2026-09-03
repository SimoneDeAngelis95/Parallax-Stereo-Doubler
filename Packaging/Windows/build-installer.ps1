param(
    [string] $BuildDirectory,
    [string] $Configuration = "Release",
    [string] $InnoCompiler
)

$ErrorActionPreference = "Stop"

$scriptDirectory = $PSScriptRoot
$projectDirectory = (Resolve-Path (Join-Path $scriptDirectory "..\..")).Path

if ([string]::IsNullOrWhiteSpace($BuildDirectory)) {
    $BuildDirectory = Join-Path $projectDirectory "build-vs"
}

if (-not (Test-Path $BuildDirectory -PathType Container)) {
    throw "Build directory not found: $BuildDirectory"
}

# Inno Setup resolves relative Source paths from the .iss file directory.
# Always pass it an absolute build path instead.
$BuildDirectory = (Resolve-Path $BuildDirectory).Path

$cmakeContents = Get-Content (Join-Path $projectDirectory "CMakeLists.txt") -Raw
$versionMatch = [regex]::Match($cmakeContents, '(?m)^\s*VERSION\s+([0-9]+\.[0-9]+\.[0-9]+)')

if (-not $versionMatch.Success) {
    throw "Could not read the project version from CMakeLists.txt."
}

$version = $versionMatch.Groups[1].Value
$pluginSource = Join-Path $BuildDirectory "Parallax_artefacts\$Configuration\VST3\Parallax.vst3"
$outputDirectory = Join-Path $scriptDirectory "output"
$issFile = Join-Path $scriptDirectory "Parallax.iss"

if (-not (Test-Path $pluginSource -PathType Container)) {
    throw "Missing plugin bundle: $pluginSource`nBuild Parallax in $Configuration mode first."
}

if ([string]::IsNullOrWhiteSpace($InnoCompiler)) {
    $command = Get-Command "ISCC.exe" -ErrorAction SilentlyContinue

    if ($null -ne $command) {
        $InnoCompiler = $command.Source
    }
    else {
        $candidatePaths = @(
            (Join-Path $env:ProgramFiles "Inno Setup 7\ISCC.exe"),
            (Join-Path $env:ProgramFiles "Inno Setup 6\ISCC.exe"),
            (Join-Path ${env:ProgramFiles(x86)} "Inno Setup 7\ISCC.exe"),
            (Join-Path ${env:ProgramFiles(x86)} "Inno Setup 6\ISCC.exe")
        )

        $InnoCompiler = $candidatePaths |
            Where-Object { -not [string]::IsNullOrWhiteSpace($_) -and (Test-Path $_) } |
            Select-Object -First 1
    }
}

if ([string]::IsNullOrWhiteSpace($InnoCompiler) -or -not (Test-Path $InnoCompiler)) {
    throw "ISCC.exe was not found. Install Inno Setup or pass -InnoCompiler with its full path."
}

New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null

& $InnoCompiler `
    "/DMyAppVersion=$version" `
    "/DPluginSource=$pluginSource" `
    "/DInstallerOutput=$outputDirectory" `
    $issFile

if ($LASTEXITCODE -ne 0) {
    throw "Inno Setup failed with exit code $LASTEXITCODE."
}

$installerPath = Join-Path $outputDirectory "Parallax-$version-Windows-x64-Setup.exe"
Write-Host "Installer created:"
Write-Host $installerPath
