#!/bin/sh

export VLC_PLUGIN_PATH=lib/plugins
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib

./bin/client
