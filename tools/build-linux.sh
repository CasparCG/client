#!/bin/sh
cd "$(dirname "$0")"

fail()
{
    echo "$1" 1>&2
    exit 1
}

# Fail early if environment not set
[ -z "$BUILD_ARCHIVE_NAME" ] && export BUILD_ARCHIVE_NAME="CasparCG Client"

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

echo Building...
/usr/bin/time -f 'Build time %E' cmake --build . --parallel  || fail "make failed"

echo Packaging...
make bundle
