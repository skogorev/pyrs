#!/bin/bash

git submodule update
cd 3rdparty
mkdir include
mkdir lib

# Building haywire
cd haywire
./compile_dependencies.sh
./compile_make.sh
cp build/libhaywire.a ../lib/
cp -R include/ ../include
cp -R lib/libuv/.libs/ ../lib/
cp -R lib/libuv/.libs/include/ ../include
cd ..

cd ..
echo "Built"
