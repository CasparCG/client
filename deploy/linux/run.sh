#!/bin/sh

export LD_LIBRARY_PATH=bin:$LD_LIBRARY_PATH

./bin/client -style plastique "$@"
