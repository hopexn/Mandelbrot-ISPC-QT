#!/usr/bin/env bash
cd ispc
ispc Mandelbrot.ispc --target=avx2-i32x8 -o Mandelbrot.o -h Mandelbrot.h
cd ..
mkdir -p build
cd build
cmake ..
make -j4
cd ..
./build/Mandelbrot