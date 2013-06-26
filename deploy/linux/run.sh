#!/bin/sh

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib
export LD_LIBRARY_PATH

./bin/client
