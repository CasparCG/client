#!/bin/sh

fail()
{
    echo "$1" 1>&2
    exit 1
}

# Fail early if environment not set
[ -z "$BUILD_ARCHIVE_NAME" ] && fail "BUILD_ARCHIVE_NAME has to be set"
[ -z "$BUILD_PARALLEL_THREADS" ] && fail "BUILD_PARALLEL_THREADS has to be set"

# Clean and enter shadow build folder
echo Cleaning...
if [ -e ../build ]; then
    rm -Rf ../build || fail "Could not delete ../build"
fi

mkdir ../build || fail "Could not create ../build"
cd ../build || fail "Could not enter ../build"

# Run qmake
echo Running qmake...
qmake ../src/Solution.pro -r -spec linux-g++-64 "CONFIG+=release" || fail "qmake failed"

# Run make using the number of hardware threads in BUILD_PARALLEL_THREADS
echo Building...
export MAKE_COMMAND="make -j$BUILD_PARALLEL_THREADS"
/usr/bin/time -f 'Build time %E' make || fail "make failed"

# Create client folder to later zip
export CLIENT_FOLDER="CasparCG Client"
if [ -f "$CLIENT_FOLDER" ]; then
    rm -Rf "$CLIENT_FOLDER" || fail "Could not delete $CLIENT_FOLDER"
fi
mkdir "$CLIENT_FOLDER" || fail "Could not create $CLIENT_FOLDER"
mkdir "$CLIENT_FOLDER/bin" || fail "Could not create $CLIENT_FOLDER/bin"
mkdir "$CLIENT_FOLDER/lib" || fail "Could not create $CLIENT_FOLDER/lib"

# Copy compiled binaries
echo Copying binaries...
find . -name \*.so\* -exec cp -fP {} "$CLIENT_FOLDER/lib/" \; || fail "Could not copy client libraries"
cp -f Shell/shell "$CLIENT_FOLDER/bin/client" || fail "Could not copy client executable"

# Copy binary dependencies
echo Copying binary dependencies...
cp -RfP ../deploy/linux/* "$CLIENT_FOLDER/" || fail "Could not copy binary dependencies"

# Copy documentation
echo Copying documentation...
cp -f ../CHANGES.TXT "$CLIENT_FOLDER/" || fail "Could not copy CHANGES.TXT"
cp -f ../LICENSE.TXT "$CLIENT_FOLDER/" || fail "Could not copy LICENSE.TXT"

# Create tar.gz file
echo Creating tag.gz...
tar -cvzf "$BUILD_ARCHIVE_NAME.tar.gz" "$CLIENT_FOLDER" || fail "Could not create archive"

