#!/usr/bin/env bash
set -euo pipefail

# --- Configurable list of Python packages required at runtime ---
REQUIRED_PY_PKGS=(scikit-learn scikit-fuzzy scikit-image pandas seaborn pillow)

# --- Helper: ensure .venv exists and activate it ---
if [ ! -d ".venv" ]; then
  echo "Creating Python virtual environment at .venv ..."
  python3 -m venv .venv
fi

# Activate venv (bash/zsh)
# shellcheck disable=SC1091
source .venv/bin/activate

# Upgrade pip/setuptools quietly and install required packages (best-effort)
# We install with dependencies so scikit-learn is complete
echo "Ensuring Python runtime packages are installed in .venv ..."
python -m pip install --upgrade pip setuptools wheel >/dev/null
python -m pip install "${REQUIRED_PY_PKGS[@]}" || {
  echo "Warning: pip install failed for some packages. See pip output above."
}

# --- Helpful environment variables to reduce GTK / fontconfig / OpenGL issues ---
# If you're inside a nix-shell that already exports better values, these won't hurt.
export GDK_BACKEND=${GDK_BACKEND:-x11}
export FONTCONFIG_PATH=${FONTCONFIG_PATH:-/etc/fonts}
# If you know where libGL/libdecor live (e.g. Nix store), add them to LD_LIBRARY_PATH here:
# export LD_LIBRARY_PATH="/run/current-system/sw/lib:${LD_LIBRARY_PATH:-}"

echo ""
echo "Environment info:"
echo "  Python: $(python --version)"
python -c "import sys, site; print('  sys.executable:', sys.executable)"
python - <<'PY' 2>/dev/null || true
try:
    import sklearn, skfuzzy, skimage
    print("  scikit-learn:", sklearn.__version__)
    print("  scikit-fuzzy: available")
    print("  scikit-image: available")
except Exception as e:
    print("  Python package check FAILED:", str(e))
PY
echo ""

# --- Build ---
mkdir -p build
echo "Building fuzzy 3D cube renderer (Modular Version)..."

# Note: keep your original g++ command; we try to reuse python3-config if available.
# If python3-config isn't available inside the venv, the build may still work (system python used).
PYTHON_CONFIG_INCLUDES=""
PYTHON_CONFIG_LDFLAGS=""
if command -v python3-config >/dev/null 2>&1; then
  PYTHON_CONFIG_INCLUDES="$(python3-config --includes)"
  PYTHON_CONFIG_LDFLAGS="$(python3-config --ldflags --embed 2>/dev/null || python3-config --ldflags)"
fi

g++ src/main.cpp \
    src/FuzzyCubeApp.cpp \
    vendor/imgui/imgui.cpp \
    vendor/imgui/imgui_draw.cpp \
    vendor/imgui/imgui_tables.cpp \
    vendor/imgui/imgui_widgets.cpp \
    vendor/imgui/backends_local/imgui_impl_glfw.cpp \
    vendor/imgui/backends_local/imgui_impl_opengl3.cpp \
    -Iinclude \
    -Ivendor/imgui \
    -Ivendor/imgui/backends_local \
    ${PYTHON_CONFIG_INCLUDES} \
    -lglfw \
    -ldl \
    -lGL \
    -lX11 \
    -lpthread \
    -lXrandr \
    -lXi \
    -lGLEW \
    $(pkg-config --cflags --libs glm) \
    ${PYTHON_CONFIG_LDFLAGS} \
    -o build/app

echo "✅ Build successful!"
echo ""
echo "Starting application..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Run the built binary (it will use the active venv via PYTHONHOME/PATH if necessary)
./build/app




# #!/usr/bin/env bash
# set -e  # Exit on error

# # Create build directory if it doesn't exist
# mkdir -p build

# # Build the project
# echo "Building fuzzy 3D cube renderer (Modular Version)..."

# g++ src/main.cpp \
#     src/FuzzyCubeApp.cpp \
#     vendor/imgui/imgui.cpp \
#     vendor/imgui/imgui_draw.cpp \
#     vendor/imgui/imgui_tables.cpp \
#     vendor/imgui/imgui_widgets.cpp \
#     vendor/imgui/backends_local/imgui_impl_glfw.cpp \
#     vendor/imgui/backends_local/imgui_impl_opengl3.cpp \
#     -Iinclude \
#     -Ivendor/imgui \
#     -Ivendor/imgui/backends_local \
#     $(python3-config --includes) \
#     -lglfw \
#     -ldl \
#     -lGL \
#     -lX11 \
#     -lpthread \
#     -lXrandr \
#     -lXi \
#     -lGLEW \
#     $(pkg-config --cflags --libs glm) \
#     $(python3-config --ldflags --embed) \
#     -o build/app

# echo "✅ Build successful!"
# echo ""
# echo "Starting application..."
# echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
# ./build/app



# #!/bin/bash

# # Create build directory if it doesn't exist
# mkdir -p build

# # Build the project
# echo "Building fuzzy 3D cube renderer (Modular Version)..."

# g++ src/main.cpp \
#     src/FuzzyCubeApp.cpp \
#     vendor/imgui/imgui.cpp \
#     vendor/imgui/imgui_draw.cpp \
#     vendor/imgui/imgui_tables.cpp \
#     vendor/imgui/imgui_widgets.cpp \
#     vendor/imgui/backends_local/imgui_impl_glfw.cpp \
#     vendor/imgui/backends_local/imgui_impl_opengl3.cpp \
#     -Iinclude \
#     -Ivendor/imgui \
#     -Ivendor/imgui/backends_local \
#     $(python3.13-config --includes) \
#     -lglfw \
#     -ldl \
#     -lGL \
#     -lX11 \
#     -lpthread \
#     -lXrandr \
#     -lXi \
#     -lGLEW \
#     -lglm \
#     $(python3.13-config --ldflags) \
#     -lpython3.13 \
#     -o build/app

# if [ $? -eq 0 ]; then
#     echo "Build successful! Running application..."
#     echo "Note: Make sure you have the required dependencies installed:"
#     echo "  - GLFW development libraries"
#     echo "  - OpenGL development libraries"
#     echo "  - X11 development libraries"
#     echo "  - Python 3.13 development headers"
#     echo "  - GLM (OpenGL Mathematics) headers"
#     echo ""
#     echo "On Arch Linux, you can install them with:"
#     echo "  sudo pacman -S glfw-x11 glfw-wayland mesa libx11 libxrandr libxi python"
#     echo ""
#     echo "For GLM (if not available in system packages):"
#     echo "  sudo pacman -S glm"
#     echo ""
#     echo "Starting application..."
#     ./build/app
# else
#     echo "Build failed!"
#     exit 1
# fi
