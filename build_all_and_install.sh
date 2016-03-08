#!/bin/bash

./build_3rdparty.sh
./build_pyrs.sh
cp build/pyrs /usr/local/bin/
