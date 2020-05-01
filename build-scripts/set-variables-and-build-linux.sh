#!/bin/sh

export BUILD_QT_PATH=~/Qt/Qt-5.14.2/5.14.2/gcc_64/bin
export BUILD_ARCHIVE_NAME="CasparCG Client"
export BUILD_PARALLEL_THREADS=8

./build-linux.sh

