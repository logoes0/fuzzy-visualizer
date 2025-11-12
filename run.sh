#!/usr/bin/env bash
set -e

# Check for diagnostic mode
if [ "$1" = "--diagnose" ] || [ "$1" = "-d" ]; then
  echo "Running diagnostics..."
  ./diagnose.sh
  exit 0
fi

# Check for software rendering mode
if [ "$1" = "--software" ] || [ "$1" = "-s" ]; then
  echo "Enabling software rendering (LIBGL_ALWAYS_SOFTWARE=1)"
  export LIBGL_ALWAYS_SOFTWARE=1
fi

# Ensure we're using the venv Python
if [ ! -d ".venv" ]; then
  echo "Error: Virtual environment not found. Please run 'nix-shell' first or create .venv manually."
  exit 1
fi

# Activate venv to get the correct Python
source .venv/bin/activate

# Get Python version and config
PYTHON_VERSION=$(python --version 2>&1 | grep -oP '\d+\.\d+' | head -n1)
echo "Using Python $PYTHON_VERSION from venv"

# Get Python config paths
if command -v python3-config >/dev/null 2>&1; then
  PYTHON_INCLUDES="$(python3-config --includes)"
  PYTHON_LDFLAGS="$(python3-config --ldflags --embed 2>/dev/null || python3-config --ldflags)"
else
  echo "Error: python3-config not found. Make sure Python development headers are installed."
  exit 1
fi

# Create build directory if it doesn't exist
mkdir -p build

# Build the project
echo "Building fuzzy 3D cube renderer (Modular Version)..."
echo "Python includes: $PYTHON_INCLUDES"
echo "Python ldflags: $PYTHON_LDFLAGS"

g++ src/main.cpp \
src/FuzzyCubeApp.cpp \
vendor/imgui/imgui.cpp \
vendor/imgui/imgui_draw.cpp \
vendor/imgui/imgui_tables.cpp \
vendor/imgui/imgui_widgets.cpp \
vendor/imgui/backends/imgui_impl_glfw.cpp \
vendor/imgui/backends/imgui_impl_opengl3.cpp \
-Iinclude \
-Ivendor/imgui \
${PYTHON_INCLUDES} \
-lglfw \
-ldl \
-lGL \
-lX11 \
-lpthread \
-lXrandr \
-lXi \
-lGLEW \
$(pkg-config --cflags --libs glm) \
${PYTHON_LDFLAGS} \
-o build/app

echo "✅ Build successful!"
echo ""

# Unset PYTHONHOME if it was set - it breaks venv's ability to find standard library
# Python will automatically detect the correct paths when PYTHONHOME is not set
unset PYTHONHOME

# Set PYTHONPATH to include current directory (for fuzzy_module.py)
export PYTHONPATH=".:${PYTHONPATH:-}"

# Get the base Python installation path (not venv) for info
PYTHON_BASE=$(python -c 'import sys; print(sys.base_prefix)' 2>/dev/null || python -c 'import sys; print(sys.prefix)')

echo "Python environment:"
echo "  Python executable: $(which python)"
echo "  Python version: $(python --version)"
echo "  Python base: $PYTHON_BASE"
echo "  Python prefix: $(python -c 'import sys; print(sys.prefix)')"
echo "  PYTHONPATH: $PYTHONPATH"
echo "  PYTHONHOME: ${PYTHONHOME:-<not set>}"
echo ""

echo "Starting application..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Disable threading in numpy/scipy BLAS libraries to avoid conflicts with OpenGL
export OMP_NUM_THREADS=1
export OPENBLAS_NUM_THREADS=1
export MKL_NUM_THREADS=1
export NUMEXPR_NUM_THREADS=1
export VECLIB_MAXIMUM_THREADS=1

# Preload Python shared library to avoid ABI issues
PYTHON_LIB=$(python -c "import sys, os; print(os.path.join(sys.base_prefix, 'lib', f'libpython{sys.version_info.major}.{sys.version_info.minor}.so'))")
if [ -f "$PYTHON_LIB" ]; then
  echo "Preloading Python library: $PYTHON_LIB"
  export LD_PRELOAD="$PYTHON_LIB:${LD_PRELOAD:-}"
fi

echo "Thread settings: OMP_NUM_THREADS=$OMP_NUM_THREADS"

./build/app