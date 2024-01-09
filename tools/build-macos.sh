#!/bin/sh
cd "$(dirname "$0")"

fail()
{
    echo "$1" 1>&2
    exit 1
}

# Fail early if environment not set
[ -z "$BUILD_QT_PATH" ] && export BUILD_QT_PATH=~/Qt/6.5.3
[ -z "$BUILD_ARCHIVE_NAME" ] && export BUILD_ARCHIVE_NAME="CasparCG Client"
[ -z "$BUILD_PARALLEL_THREADS" ] && export BUILD_PARALLEL_THREADS=$(sysctl -n hw.logicalcpu)
export BUILD_HDIUTILS_WORKAROUND_SIZE="180m"

export ARCH=$1
if [[ "$ARCH" != "x86_64" && "$ARCH" != "arm64" ]]; then
    echo "$ARCH is not a supported cpu architecture, only arm64 and x86_64 are allowed."
    exit 1
fi

# Clean and enter shadow build folder
echo Cleaning...
if [ -e ../build ]; then
    rm -Rf ../build || fail "Could not delete ../build"
fi

mkdir ../build || fail "Could not create ../build"
cd ../build || fail "Could not enter ../build"

# Run qmake
echo Running cmake...
cmake ../src -DQt6_ROOT=$BUILD_QT_PATH/macos -DCMAKE_OSX_ARCHITECTURES=$ARCH

# Run make using the number of hardware threads in BUILD_PARALLEL_THREADS
echo Building...
export MAKE_COMMAND="make -j$BUILD_PARALLEL_THREADS"
/usr/bin/time make || fail "make failed"

# Create app bundle
echo Creating app bundle...
cmake --install .

# Create client folder to later dmg
export CLIENT_FOLDER="CasparCG Client"
if [ -f "$CLIENT_FOLDER" ]; then
    rm -Rf "$CLIENT_FOLDER" || fail "Could not delete $CLIENT_FOLDER"
fi
mkdir "$CLIENT_FOLDER" || fail "Could not create $CLIENT_FOLDER"

# Copy app bundle
mv staging/casparcg-client.app "$CLIENT_FOLDER/$CLIENT_FOLDER.app" || fail "Could not move app bundle"

# Sign app bundle
if [ -n "$MACOS_SIGN_NAME" ]; then
    echo "Signing..."
    codesign --deep --timestamp --options runtime -s "$MACOS_SIGN_NAME" --entitlements "../resources/macos/entitlements.plist" "$CLIENT_FOLDER/$CLIENT_FOLDER.app"
fi

# Copy documentation
echo Copying documentation...
cp -f ../CHANGELOG "$CLIENT_FOLDER/" || fail "Could not copy CHANGELOG"
cp -f ../LICENSE "$CLIENT_FOLDER/" || fail "Could not copy LICENSE"
ln -s /Applications "$CLIENT_FOLDER/"

# Create dmg file
echo Creating dmg...
DMG_NAME="$BUILD_ARCHIVE_NAME-macos-$ARCH.dmg"
hdiutil create -size "$BUILD_HDIUTILS_WORKAROUND_SIZE" -volname "$CLIENT_FOLDER" -srcfolder "$CLIENT_FOLDER" -ov -format UDZO "$DMG_NAME" || fail "Could not create dmg"
 
# notarize dmg
if [ -n "$APPLEID" ]; then
    echo "Notarizing..."
    xcrun notarytool submit "$DMG_NAME" --wait --apple-id "$APPLEID" --password "$APPLEIDPASS" --team-id "$APPLEIDTEAM"
    xcrun stapler staple "$DMG_NAME"
fi

