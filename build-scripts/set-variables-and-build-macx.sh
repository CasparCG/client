#!/bin/sh

export BUILD_QT_PATH=$HOME/Qt/Qt-5.14.2/5.14.2/clang_64/bin
export BUILD_ARCHIVE_NAME="CasparCG Client"
export BUILD_PARALLEL_THREADS=$(sysctl -n hw.logicalcpu)
export BUILD_HDIUTILS_WORKAROUND_SIZE="130m"

./build-macx.sh

