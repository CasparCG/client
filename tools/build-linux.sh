#!/bin/sh
cd "$(dirname "$0")"

fail()
{
    echo "$1" 1>&2
    exit 1
}

# Fail early if environment not set
[ -z "$BUILD_ARCHIVE_NAME" ] && export BUILD_ARCHIVE_NAME="CasparCG Client"
[ -z "$BUILD_PARALLEL_THREADS" ] && export BUILD_PARALLEL_THREADS=8

# Clean and enter shadow build folder
echo Cleaning...
if [ -e ../build ]; then
    rm -Rf ../build || fail "Could not delete ../build"
fi

mkdir ../build || fail "Could not create ../build"
cd ../build || fail "Could not enter ../build"

# Run qmake
echo Running cmake...
cmake ../src/ || fail "cmake failed"

# Run make using the number of hardware threads in BUILD_PARALLEL_THREADS
echo Building...
export MAKE_COMMAND="make -j$BUILD_PARALLEL_THREADS"
/usr/bin/time -f 'Build time %E' make || fail "make failed"

echo Packaging...
make bundle
