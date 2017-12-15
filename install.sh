#!/bin/bash

mkdir cmake-build
cd cmake-build
cmake ..
make

echo "Main program : src/Tl2017"
echo "Tests : src/Tl2017_test"
