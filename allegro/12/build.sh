#!/usr/bin/env bash

PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig cmake -S. -Bbuild
cmake --build build

exit 0
