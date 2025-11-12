{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Python 3.13 (matching your venv)
    python313
    python313Packages.pip
    python313Packages.virtualenv
    python313Packages.packaging
    
    # Core Python packages (pre-installed from Nix)
    python313Packages.numpy
    python313Packages.scipy
    python313Packages.networkx
    python313Packages.scikit-learn
    
    # Graphics/OpenGL libraries (project-specific)
    glfw
    glew
    mesa
    libGL
    libGLU
    glm
    wayland    
    # X11 libraries (project-specific)
    xorg.libX11
    xorg.libXrandr
    xorg.libXi
    xorg.libXcursor
    xorg.libXinerama
    
    # Build tool for pkg-config
    pkg-config
    
    # System libraries needed by Python packages (numpy, pandas, etc.)
    zlib
    bzip2
    xz
  ];
  
  # Set up environment variables for OpenGL and graphics libraries
  shellHook = ''
    echo "🚀 OpenGL Python/C++ Development Environment"
    echo "Python: $(python --version)"
    echo "GCC: $(gcc --version | head -n1)"
    echo ""
    
    # Remove old virtual environment if it exists with wrong Python version
    if [ -d .venv ]; then
      VENV_PYTHON_VERSION=$(.venv/bin/python --version 2>&1 | grep -oP '\d+\.\d+' || echo "unknown")
      NIX_PYTHON_VERSION=$(python --version 2>&1 | grep -oP '\d+\.\d+')
      
      if [ "$VENV_PYTHON_VERSION" != "$NIX_PYTHON_VERSION" ]; then
        echo "⚠️  Virtual environment Python version mismatch detected"
        echo "   venv: Python $VENV_PYTHON_VERSION"
        echo "   nix:  Python $NIX_PYTHON_VERSION"
        echo "   Recreating virtual environment..."
        rm -rf .venv
      fi
    fi
    
    # Create and activate virtual environment if it doesn't exist
    if [ ! -d .venv ]; then
      echo "Creating Python virtual environment with Python 3.13..."
      python -m venv .venv
    fi
    
    source .venv/bin/activate
    
    # Upgrade pip
    pip install --quiet --upgrade pip 2>/dev/null || true
    
    # Install required packages
    echo "Ensuring Python runtime packages are installed in .venv ..."
    pip install --quiet scikit-fuzzy scikit-learn scikit-image 2>/dev/null || true
    
    echo ""
    echo "Available libraries:"
    echo "  - GLFW (OpenGL window management)"
    echo "  - Mesa (OpenGL implementation)"
    echo "  - GLM (OpenGL Mathematics)"
    echo "  - Python packages: numpy, scipy, scikit-fuzzy, scikit-learn"
    echo ""
    echo "Virtual environment activated at .venv/"
    echo "Python interpreter: $(which python)"
    echo ""
    echo "Environment info:"
    echo "  Python: $(python --version)"
    echo "  sys.executable: $(python -c 'import sys; print(sys.executable)')"
    echo "  scikit-learn: $(python -c 'import sklearn; print(sklearn.__version__)' 2>/dev/null || echo 'not installed')"
    echo "  scikit-fuzzy: $(python -c 'import skfuzzy; print("available")' 2>/dev/null || echo 'not installed')"
    echo "  scikit-image: $(python -c 'import skimage; print("available")' 2>/dev/null || echo 'not installed')"
    echo ""
    
    # Set library paths for dynamic linking (including system libraries for Python packages)
    export LD_LIBRARY_PATH="${pkgs.libGL}/lib:${pkgs.mesa}/lib:${pkgs.glfw}/lib:${pkgs.zlib}/lib:${pkgs.bzip2}/lib:${pkgs.xz}/lib:${pkgs.stdenv.cc.cc.lib}/lib:$LD_LIBRARY_PATH"
    
    # Ensure Python from venv is used
    export PATH="$PWD/.venv/bin:$PATH"
  '';
  
  # Environment variables for pkg-config to find libraries
  PKG_CONFIG_PATH = "${pkgs.glfw}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig";
}