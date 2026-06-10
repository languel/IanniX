About IanniX
============
This is a personal fork of the original [IanniX](https://github.com/buzzinglight/IanniX) project, a graphical real-time open-source sequencer for digital art.  
IanniX is a graphical open-source sequencer, based on Iannis Xenakis works, for digital art. IanniX syncs via Open Sound Control (OSC) events and curves to your real-time environment.

**Download:** signed & notarized macOS (Apple Silicon) builds are on the [releases page](https://github.com/languel/IanniX/releases).

## Changes in this fork

### Build / platform
- Updated for Apple Silicon (arm64): builds with Homebrew Qt 5.15 on current macOS
- Syphon.framework built from source for arm64 and vendored in `frameworks/`
- Syphon client ported to the modern `SyphonOpenGLClient` API (enabling Syphon-In crashed with the legacy API)
- Hardened-runtime codesigning + notarization script: `Tools/build-macos.sh`
- Restored `icons/IanniX.qrc` (accidentally removed by an over-broad `.gitignore`)

### UI / theming
- Coherent dark and light themes for the whole interface (toolbar, inspector, transport, helper, script editor), switched live with the sun toolbar button — the toggle previously only changed the score canvas colors
- Theme engine in `misc/uitheme.cpp`: one parameterized stylesheet + per-theme palette applied app-wide (Fusion base style). Do not add color-bearing stylesheets to `.ui` files — widget-level stylesheets block app-stylesheet updates on theme switches
- Script editor: dark/light syntax color schemes, theme-following help pane, line-number gutter no longer collides with the scrollbar
- Compact playback-speed slider next to the speed field
- Keyboard shortcuts: `Cmd+Shift+C` adds a cursor; `Cmd+,` opens Preferences (Config tab), revealing the Inspector if it was closed

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
