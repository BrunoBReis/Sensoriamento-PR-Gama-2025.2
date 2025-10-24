#!/usr/bin/env bash

set -e # to stop on first error

# Go to root directory of project
# Could be run from anywhere
cd "$(dirname "$0")/.."

# Define build directory
BUILD_DIR="build"

# Create build directory if not existis
mkdir -p "$BUILD_DIR"

# Runing cmake and compile
echo "Running cmake..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Building project..."
cmake --build "$BUILD_DIR" -j$(nproc)

echo "Build Complete!"
