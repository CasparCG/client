#!/bin/sh
cd "$(dirname "$0")"

fail()
{
    echo "$1" 1>&2
    exit 1
}

[ -z "$BUILD_QT_PATH" ] && export BUILD_QT_PATH=~/Qt/6.5.3
[ -z "$BUILD_PARALLEL_THREADS" ] && export BUILD_PARALLEL_THREADS=$(sysctl -n hw.logicalcpu)
[ -z "$MACOSX_DEPLOYMENT_TARGET" ] && export MACOSX_DEPLOYMENT_TARGET=13

BUILD_DIR="../build-boost"
if [ -e "$BUILD_DIR" ]; then
    rm -Rf "$BUILD_DIR" || fail "Could not delete $BUILD_DIR"
fi
mkdir "$BUILD_DIR" || fail "Could not create $BUILD_DIR"
cd "$BUILD_DIR" || fail "Could not enter $BUILD_DIR"

cmake ../src \
    -DQt6_ROOT=$BUILD_QT_PATH/macos \
    -DBOOST_USE_PRECOMPILED=OFF \
    || fail "cmake configure failed"

cmake --build . --target boost -j$BUILD_PARALLEL_THREADS || fail "boost build failed"

ZIP_NAME="boost_1_83_0-macos-multiarch-release.zip"
cd boost-install || fail "Could not find boost-install directory"
zip -r "../../tools/$ZIP_NAME" . || fail "Could not create zip"

echo ""
echo "Created tools/$ZIP_NAME"
echo "Upload this to the download mirror at: tools/$ZIP_NAME"
