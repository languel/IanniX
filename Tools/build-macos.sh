#!/bin/zsh
# Build, sign, and notarize IanniX for macOS (Apple Silicon).
#
# Prerequisites:
#   brew install qt@5
#   Xcode + Metal toolchain (xcodebuild -downloadComponent MetalToolchain)
#   frameworks/Syphon.framework (built from https://github.com/Syphon/Syphon-Framework)
#   A "Developer ID Application" certificate in the keychain
#   notarytool keychain profile (xcrun notarytool store-credentials <profile>)
#
# Usage: Tools/build-macos.sh [notary-profile]
set -e

QT=/opt/homebrew/opt/qt@5
IDENTITY="Developer ID Application: liubomir borissov (K39T7B8529)"
PROFILE="${1:-UPBGE_NOTARY}"
VERSION=$(plutil -extract CFBundleShortVersionString raw Info.plist)
ROOT=$(cd "$(dirname "$0")/.." && pwd)

cd "$ROOT"
mkdir -p build && cd build

echo "== Building IanniX $VERSION =="
"$QT/bin/qmake" ../IanniX.pro CONFIG+=sdk_no_version_check
make -j"$(sysctl -n hw.ncpu)"

echo "== Bundling Qt frameworks =="
"$QT/bin/macdeployqt" IanniX.app

echo "== Signing =="
find IanniX.app/Contents/Frameworks -name "*.dylib" -maxdepth 1 \
    -exec codesign --force --options runtime --timestamp -s "$IDENTITY" {} \;
for fw in IanniX.app/Contents/Frameworks/*.framework; do
    codesign --force --options runtime --timestamp -s "$IDENTITY" "$fw"
done
find IanniX.app/Contents/PlugIns -name "*.dylib" \
    -exec codesign --force --options runtime --timestamp -s "$IDENTITY" {} \;
codesign --force --options runtime --timestamp \
    --entitlements ../entitlements.plist -s "$IDENTITY" IanniX.app
codesign --verify --deep --strict IanniX.app

echo "== Notarizing app =="
ditto -c -k --keepParent IanniX.app IanniX-notarize.zip
xcrun notarytool submit IanniX-notarize.zip --keychain-profile "$PROFILE" --wait
xcrun stapler staple IanniX.app
rm IanniX-notarize.zip

echo "== Creating DMG =="
DMG="IanniX-$VERSION-macOS-arm64.dmg"
STAGE=$(mktemp -d)
cp -R IanniX.app "$STAGE/"
ln -s /Applications "$STAGE/Applications"
hdiutil create -volname "IanniX $VERSION" -srcfolder "$STAGE" -ov -format UDZO "$DMG"
rm -rf "$STAGE"

codesign --force --timestamp -s "$IDENTITY" "$DMG"
xcrun notarytool submit "$DMG" --keychain-profile "$PROFILE" --wait
xcrun stapler staple "$DMG"
spctl -a -t open --context context:primary-signature -v "$DMG"

echo "== Done: build/$DMG =="
