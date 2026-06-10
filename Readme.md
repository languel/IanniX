About IanniX
============
This is a personal fork of the original [IanniX](https://github.com/buzzinglight/IanniX) project, a graphical real-time open-source sequencer for digital art.  
IanniX is a graphical open-source sequencer, based on Iannis Xenakis works, for digital art. IanniX syncs via Open Sound Control (OSC) events and curves to your real-time environment.

Getting started
===============
1. Download the latest `IanniX-…-macOS-arm64.dmg` from the [releases page](https://github.com/languel/IanniX/releases)
2. Open the DMG and drag **IanniX** into **Applications**
3. Launch it and open a score from **FILES → Examples** in the Inspector panel — press play
4. Toggle light/dark with the ☀ button in the toolbar; `Cmd+,` opens the preferences (Config tab)

That's it — no extra installs needed. To send/receive OSC, MIDI, or Syphon video, see the **CONFIG** tab in the Inspector.

> ⚠️ **Platform support:** this fork is developed and tested **only on Apple Silicon Macs** (arm64, recent macOS). The changes are not intentionally platform-specific, but Windows, Linux, and Intel Macs are untested here and may have regressed — use [upstream](https://github.com/buzzinglight/IanniX) for those platforms.

## Branches
- `main` — stable; releases are built from here
- `dev-macos-arm64` — ongoing work (Apple Silicon focus)
- `pr/*` — single-topic branches based on upstream `master`, intended for upstream pull requests

## Changes in this fork

### Build / platform
- Updated for Apple Silicon (arm64): builds with Homebrew Qt 5.15 on current macOS
- Syphon.framework built from source for arm64 and vendored in `frameworks/`
- Syphon client ported to the modern `SyphonOpenGLClient` API (enabling Syphon-In crashed with the legacy API)
- Hardened-runtime codesigning + notarization script: `Tools/build-macos.sh`
- Restored `icons/IanniX.qrc` (accidentally removed by an over-broad `.gitignore`)

### Content & files
- Bundled Examples and Patches are copied into `~/Documents/IanniX` at launch (missing files only — edits are preserved; delete a file to restore the original). Saving into the signed app bundle would break its code signature
- File browser shows one coherent tree (your Documents/IanniX folder); right-click any file or folder to reveal it in Finder
- The accent color is configurable: INFOS → RESOURCES → COLORS → `gui_accent`

### UI / theming
- Coherent dark and light themes for the whole interface (toolbar, inspector, transport, helper, script editor), switched live with the sun toolbar button — the toggle previously only changed the score canvas colors
- Theme engine in `misc/uitheme.cpp`: one parameterized stylesheet + per-theme palette applied app-wide (Fusion base style). Do not add color-bearing stylesheets to `.ui` files — widget-level stylesheets block app-stylesheet updates on theme switches
- Configurable interface accent color (`gui_accent` in INFOS → RESOURCES → COLORS, default muted steel blue); a RESTORE DEFAULT COLORS button resets the palette
- Script editor: dark/light syntax color schemes, theme-following help pane, line-number gutter no longer collides with the scrollbar
- Transport: playback rate as a compact number box next to the clock
- Keyboard shortcuts: `Cmd+0` resets the view to 2D (object editing is disabled while the camera is rotated); `Cmd+Shift+C` adds a cursor; `Cmd+,` opens Preferences (Config tab), revealing the Inspector if it was closed

### Stability
- Fixed a crash (use-after-free) when opening scores after the file browser refreshed itself
- The file browser recovers automatically if the first launch races the macOS Documents-folder permission prompt

### Using Syphon
- **Out** (CONFIG → SOFTWARE → Enable Syphon Out): publishes the score render as a Syphon server for any client (MadMapper, OBS, VDMX…)
- **In** (Enable Syphon In): registers the first available Syphon source as a live texture named `syphon`. Show it on a trigger with `run("settexture <id> syphon");` from the script editor (or the equivalent message over OSC)

## License
This project is licensed under the GNU General Public License (GPL), as per the original repository.

Documentation
============
https://github.com/buzzinglight/IanniX/wiki

Build IanniX
============

macOS (Apple Silicon)
---------------------
- `brew install qt@5`
- Xcode with the Metal toolchain (`xcodebuild -downloadComponent MetalToolchain`) — only needed to rebuild Syphon
- Syphon.framework is vendored in `frameworks/` (built from https://github.com/Syphon/Syphon-Framework for arm64+x86_64)
- Build + sign + notarize + DMG: `Tools/build-macos.sh` (requires a Developer ID certificate and a notarytool keychain profile)
- Or manually:

        mkdir build && cd build
        /opt/homebrew/opt/qt@5/bin/qmake ../IanniX.pro CONFIG+=sdk_no_version_check
        make -j8
        /opt/homebrew/opt/qt@5/bin/macdeployqt IanniX.app
        codesign --force --deep -s - IanniX.app   # ad-hoc; required on Apple Silicon

  Note: after a code change, always delete `build/IanniX.app` before re-running `make` — relinking into an already-deployed bundle mixes two copies of Qt and the app aborts at launch.

- If you want to build IanniX with Kinect support, you'll need libfreenect (http://openkinect.org/wiki/Getting_Started)

Other platforms
---------------
- Download & install Qt5 for your platform (QtCreator included)
- Open IanniX.pro and build/run!
- Linux: install Qt via your package manager (Qt5 development + OpenGL packages) or build it from source
