#!/usr/bin/env bash

set -e # to stop on first error

# Go to root directory of project
# Could be run from anywhere
cd "$(dirname "$0")/.."

echo "Cleaning build directory..."
rm -rf build compile_commands.json
echo "Clean Complete!"
