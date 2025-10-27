#!/usr/bin/env bash

set -e # to stop on first error

# Go to root directory of project
# Could be run from anywhere
cd "$(dirname "$0")/.."

# Define source and link name
SOURCE_FILE="build/compile_commands.json"
LINK_NAME="compile_commands.json"

# Check if file exisits
if [ ! -f "$SOURCE_FILE" ]; then
  echo "Error: Source file not found at $SOURCE_FILE"
  echo "Try again"
  exit 1
fi

# Create a sym link and force it
echo "Creating a sym link $LINK_NAME -> $SOURCE_FILE"
ln -sf "$SOURCE_FILE" "$LINK_NAME"

echo "Just run :LspRestart"
