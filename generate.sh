#!/bin/bash

#export CC=/usr/bin/clang
#export CXX=/usr/bin/clang++
cd ./build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNREAL=OFF

exit 0

