#!/bin/sh

if ! command -v cmake &> /dev/null; then
    echo "cmake is not installed. Please install cmake and try again."
    exit 1
fi

mkdir -p build

cd build || { echo "Failed to change to the build directory."; exit 1; }

cmake ..
if [ $? -ne 0 ]; then
    echo "Error while running cmake."
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "Error while running make."
    exit 1
fi

sudo ./daemon config.txt
