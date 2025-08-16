#!/bin/bash

# Exit if any command fails
set -e

# Create build folder if it doesn't exist
mkdir -p build

# Get Python include and library flags
PY_INC=$(python3-config --includes)
PY_LIBS=$(python3-config --ldflags --embed)

# Compile C++ program with Python + ImGui + OpenGL
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
    $PY_INC \
    -lglfw -lGLEW -lGL \
    -o build/app \
    $PY_LIBS

# Run the executable
echo "🚀 Running..."
./build/app
