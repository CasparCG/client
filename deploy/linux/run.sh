#!/usr/bin/env sh

cd "`dirname "$0"`"

export LD_LIBRARY_PATH=bin:$LD_LIBRARY_PATH

./bin/shell "$@"
