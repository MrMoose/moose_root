#!/bin/bash

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cd ./build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNREAL=ON -DCMAKE_USER_MAKE_RULES_OVERRIDE=/home/sm/devel/moose/ClangOverrides.txt

exit 0

