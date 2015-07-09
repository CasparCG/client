#!/bin/sh

fail()
{
    echo "$1" 1>&2
    exit 1
}

BUILD_ARCHIVE_NAME="client"

# Clean and enter shadow build folder
echo Cleaning...
if [ -e ../build ]; then
    rm -Rf ../build || fail "Could not delete ../build"
fi

mkdir ../build || fail "Could not create ../build"
cd ../build || fail "Could not enter ../build"

# Run qmake
echo Running qmake...
qmake -qt=5 ../src/Solution.pro -r -spec linux-g++-64 CONFIG+=release CONFIG+=system-libqatemcontrol CONFIG+=system-boost CONFIG+=system-gpio-client CONFIG+=system-oscpack CONFIG+=system-libvlc || fail "qmake failed"

# Run make using the number of hardware threads in BUILD_PARALLEL_THREADS
echo Building...
time make -j8 || fail "make failed"

# Create client folder to later zip
CLIENT_FOLDER="$BUILD_ARCHIVE_NAME"
BIN_DIR="$CLIENT_FOLDER/usr/bin"
LIB_DIR="$CLIENT_FOLDER/usr/lib"
DOC_DIR="$CLIENT_FOLDER/usr/share/doc/casparcg-client"
if [ -f "$CLIENT_FOLDER" ]; then
    rm -Rf "$CLIENT_FOLDER" || fail "Could not delete $CLIENT_FOLDER"
fi
mkdir "$CLIENT_FOLDER" || fail "Could not create $CLIENT_FOLDER"
mkdir -p "$BIN_DIR" || fail "Could not create $BIN_DIR"
mkdir -p "$LIB_DIR" || fail "Could not create $LIB_DIR"
mkdir -p "$DOC_DIR" || fail "Could not create $DOC_DIR"

# Copy compiled binaries
echo Copying binaries...
find . -name \*.so\* -exec cp -fa {} "$LIB_DIR" \; || fail "Could not copy client libraries"
cp -fa Shell/shell "$BIN_DIR/casparcg-client" || fail "Could not copy client executable"

# Copy documentation
echo Copying documentation...
cp -fa ../CHANGES.TXT "$DOC_DIR/" || fail "Could not copy CHANGES.TXT"
cp -fa ../LICENSE.TXT "$DOC_DIR/" || fail "Could not copy LICENSE.TXT"

# Create tar.gz file
echo Creating tag.gz...
tar -cvzf "$BUILD_ARCHIVE_NAME.tar.gz" "$CLIENT_FOLDER" || fail "Could not create archive"

