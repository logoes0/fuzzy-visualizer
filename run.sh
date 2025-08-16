#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Build the project
echo "Building fuzzy 3D cube renderer..."

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
    -lglfw \
    -ldl \
    -lGL \
    -lX11 \
    -lpthread \
    -lXrandr \
    -lXi \
    -lGLEW \
    $(python3.13-config --includes) \
    $(python3.13-config --ldflags) \
    -lpython3.13 \
    -o build/app

if [ $? -eq 0 ]; then
    echo "Build successful! Running application..."
    echo "Note: Make sure you have the required dependencies installed:"
    echo "  - GLFW development libraries"
    echo "  - OpenGL development libraries"
    echo "  - X11 development libraries"
    echo "  - Python 3.13 development headers"
    echo "  - GLM (OpenGL Mathematics) headers"
    echo ""
    echo "On Arch Linux, you can install them with:"
    echo "  sudo pacman -S glfw-x11 glfw-wayland mesa libx11 libxrandr libxi python"
    echo ""
    echo "For GLM (if not available in system packages):"
    echo "  sudo pacman -S glm"
    echo ""
    echo "Starting application..."
    ./build/app
else
    echo "Build failed!"
    exit 1
fi
