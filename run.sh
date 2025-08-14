#!/bin/bash

# Exit if any command fails
set -e

# Create build folder if it doesn't exist
mkdir -p build

# Compile C++ program
echo "🔨 Compiling..."
g++ src/main.cpp \
    vendor/imgui/imgui.cpp \
    vendor/imgui/imgui_draw.cpp \
    vendor/imgui/imgui_tables.cpp \
    vendor/imgui/imgui_widgets.cpp \
    vendor/imgui/backends_local/imgui_impl_glfw.cpp \
    vendor/imgui/backends_local/imgui_impl_opengl3.cpp \
    -Iinclude \
    -Ivendor/imgui \
    -Ivendor/imgui/backends_local \
    -lglfw -lGLEW -lGL \
    -o build/app

# Run the executable
echo "🚀 Running..."
./build/app
