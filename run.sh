#!/bin/bash

# Exit if any command fails
set -e

# Create build folder if it doesn't exist
mkdir -p build

# Compile C++ program
echo "🔨 Compiling..."
g++ src/main.cpp -o build/app -Iinclude -lglfw -lGLEW -lGL

# Run the executable
echo "🚀 Running..."
./build/app
