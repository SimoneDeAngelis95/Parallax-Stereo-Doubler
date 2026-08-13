# Parallax-Stereo-Doubler

A lightweight stereo doubler plugin that creates width and depth through a controlled temporal offset between the left and right channels. Built with C++ and JUCE.

## Building

JUCE is downloaded automatically by CMake during the first configuration.

### macOS — Xcode

Generate the Xcode project:

```bash
cmake -S . -B build-xcode -G Xcode
```

Open it in Xcode:

```bash
open build-xcode/Parallax.xcodeproj
```

### Windows — Visual Studio 2022

Generate a 64-bit Visual Studio solution:

```powershell
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64
```

Open `build-vs/Parallax.sln` in Visual Studio. If a different Visual Studio
version is installed, run `cmake --help` to see the available generator name.
