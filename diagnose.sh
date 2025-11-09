#!/usr/bin/env bash
# Diagnostic script to check for OpenGL/library conflicts

echo "=== OpenGL/Library Diagnostics ==="
echo ""

echo "1. Checking for multiple libGL versions:"
ldd build/app 2>/dev/null | grep -i "libgl\|mesa" || echo "  (build/app not found)"
echo ""

echo "2. Checking Python library:"
ldd build/app 2>/dev/null | grep -i python || echo "  (No Python library linked)"
echo ""

echo "3. Checking OpenGL environment variables:"
echo "  LIBGL_ALWAYS_SOFTWARE: ${LIBGL_ALWAYS_SOFTWARE:-<not set>}"
echo "  LIBGL_DEBUG: ${LIBGL_DEBUG:-<not set>}"
echo "  MESA_DEBUG: ${MESA_DEBUG:-<not set>}"
echo ""

echo "4. Checking which OpenGL libraries are available:"
find /nix/store -name "libGL.so*" 2>/dev/null | head -5
echo ""

echo "5. Checking GPU driver info:"
glxinfo 2>/dev/null | grep -i "opengl\|vendor\|renderer" | head -10 || echo "  (glxinfo not available)"
echo ""

echo "6. Testing with software rendering:"
echo "  Try running with: LIBGL_ALWAYS_SOFTWARE=1 ./build/app"
echo ""

echo "7. Checking for recent kernel/driver errors:"
dmesg | tail -20 | grep -i "gpu\|drm\|mesa\|gl" || echo "  (No recent GPU errors)"
echo ""

