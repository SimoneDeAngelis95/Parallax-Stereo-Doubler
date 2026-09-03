# PARALLAX

### Stereo Doubler

**One signal. Two perspectives.**

![Parallax interface](Documentation/Images/parallax-interface.png)

Parallax is a stereo temporal doubler that creates width, depth and movement by
placing the left and right channels at slightly different points in time.

It is not intended to produce a conventional echo. Instead, Parallax creates two
temporal perspectives of the same signal and lets you shape the space between
them.

> **Parallax 1.0.0** is the first stable release for macOS and Windows.

## Plugin formats

| Platform | Formats          |
| -------- | ---------------- |
| macOS    | Audio Unit, VST3 |
| Windows  | VST3 x64         |

## macOS installation

Parallax is not signed or notarized, so macOS may initially block its installer.

1. Open the downloaded `.pkg`. If macOS blocks it, close the warning.
2. Open **System Settings > Privacy & Security** and scroll down to **Security**.
3. Click **Open Anyway**, enter your password and confirm.
4. Complete the installation and restart your DAW.

Only override this warning for an installer downloaded from this official
repository.

## Features

- Adjustable temporal offset from `0` to `50 ms`
- Selectable delayed side: left or right
- Stereo width control from mono to `200%`
- Organic wow, drift and flutter modulation
- Smoothed parameter changes and fractional-delay interpolation
- Mono-to-stereo and stereo-to-stereo processing
- Resizable vintage-inspired interface
- VST3 and Audio Unit formats
- Parameter automation and project-state restoration

## Controls

| Control          | Description                                                                                                  |
| ---------------- | ------------------------------------------------------------------------------------------------------------ |
| **OFFSET**       | Sets the temporal distance between the direct and delayed perspectives, from `0` to `50 ms`.                 |
| **DELAYED SIDE** | Selects whether the left or right side receives the delayed perspective.                                     |
| **SPREAD**       | Controls the stereo Side component: mono at `0%`, natural stereo at `100%`, and extended width up to `200%`. |
| **WOW**          | Adds organic wow, drift and flutter to the delayed perspective.                                              |

## Audio configurations

Parallax supports:

- mono input to stereo output;
- stereo input to stereo output.

With a mono source, Parallax duplicates the input internally and creates two
temporal perspectives. With a stereo source, it preserves the original channels
and delays the selected side.

## Building from source

### macOS — Xcode

Generate the Xcode project:

```bash
cmake -S . -B build-xcode -G Xcode
```

Or build the Release configuration from the terminal:

```bash
cmake --build build-xcode --config Release
```

Create the AU and VST3 installer with:

```bash
./Packaging/macOS/build-installer.sh
```

The resulting package is written to `Packaging/macOS/output/`.

### Windows — Visual Studio 2026

The Visual Studio 18 generator requires CMake `4.2` or newer.

Generate a 64-bit Visual Studio solution:

```powershell
cmake -S . -B build-vs -G "Visual Studio 18 2026" -A x64
```

Open `build-vs/Parallax.sln` in Visual Studio, or build from the terminal:

```powershell
cmake --build build-vs --config Release
```

### Windows installer

After installing [Inno Setup](https://jrsoftware.org/isinfo.php), run:

```powershell
.\Packaging\Windows\build-installer.ps1
```

The installer is written to `Packaging\Windows\output\` and copies
`Parallax.vst3` to `C:\Program Files\Common Files\VST3`.

If a different Visual Studio version is installed, run `cmake --help` to see the
available generator names.

By default, CMake is configured with `PARALLAX_COPY_PLUGIN_AFTER_BUILD=ON`, so a
successful build attempts to copy the generated plugin to the standard system
plugin location. It can be disabled during configuration:

```bash
cmake -S . -B build -DPARALLAX_COPY_PLUGIN_AFTER_BUILD=OFF
```

## Notes

- `SPREAD` values above `100%` intentionally exaggerate the Side component and
  may reduce mono compatibility or increase channel peaks.
- `WOW` is deliberately creative at high settings.
- Parallax does not include a limiter. Manage downstream level when using extreme
  settings.

## License

Copyright © Simone De Angelis.

Parallax is licensed under the [GNU General Public License v3.0](LICENSE).
