About IanniX
============
This is a personal fork of the original [IanniX](https://github.com/buzzinglight/IanniX) project, a graphical real-time open-source sequencer for digital art.  
IanniX is a graphical open-source sequencer, based on Iannis Xenakis works, for digital art. IanniX syncs via Open Sound Control (OSC) events and curves to your real-time environment.

## Changes
- Updated for Apple Silicon (arm64): builds with Homebrew Qt 5.15 on current macOS
- Syphon.framework built from source for arm64 and vendored in `frameworks/`
- Restored `icons/IanniX.qrc` (accidentally removed by an over-broad `.gitignore`)
- Hardened-runtime codesigning + notarization script: `Tools/build-macos.sh`

## License
This project is licensed under the GNU General Public License (GPL), as per the original repository.

Documentation
============
https://github.com/buzzinglight/IanniX/wiki

Build IanniX
============

All platforms
-------------
- Download & install Qt5 for your platform
- Download & install QtCreator (included in Qt5 package)
- Open IanniX.pro and build/run!

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

- If you want to build IanniX with Kinect support, you'll need libfreenect (http://openkinect.org/wiki/Getting_Started)

Linux
-----
- You'll need to download & build Qt before building IanniX
  - If your distribution supports it, you can also install Qt via the package
    manager, e.g.:

        aptitude install libqt4-dev libqt4-opengl-dev
