{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Python with pip for installing packages
    python311
    python311Packages.pip
    python311Packages.virtualenv
    python311Packages.packaging

    # Core Python packages (pre-installed from Nix)
    python311Packages.numpy
    python311Packages.scipy
    python311Packages.networkx

    # Add extra Python packages (non-destructive): provide common ML / data / plotting libs
    # We use python311.withPackages to avoid depending on exact attribute names in older/newer nixpkgs.
    (python311.withPackages (ps: with ps; [
      scikit_learn
      scikit_image
      seaborn
      pandas
      jupyterlab
      pillow
    ]))

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
  ];

  # Set up environment variables for OpenGL and graphics libraries
  shellHook = ''
    echo "🚀 OpenGL Python/C++ Development Environment"
    echo "Python: $(python --version)"
    echo "GCC: $(gcc --version | head -n1)"
    echo ""

    # Create and activate virtual environment if it doesn't exist
    if [ ! -d .venv ]; then
      echo "Creating Python virtual environment..."
      python -m venv .venv
    fi

    source .venv/bin/activate

    # Try to ensure key Python packages are available in the venv as a fallback.
    # We prefer Nix-provided packages (above), but in case an attribute is missing
    # we pip-install here (quietly).
    echo "Ensuring Python packages in .venv (fallback installs if needed)..."
    pip install --quiet scikit-learn scikit-image seaborn pandas jupyterlab pillow --no-deps 2>/dev/null || true

    # Install scikit-fuzzy without matplotlib (avoid tkinter issues) — keep existing pip fallback.
    echo "Installing scikit-fuzzy..."
    pip install --quiet scikit-fuzzy --no-deps 2>/dev/null || true

    echo ""
    echo "Available libraries:"
    echo "  - GLFW (OpenGL window management)"
    echo "  - Mesa (OpenGL implementation)"
    echo "  - GLM (OpenGL Mathematics)"
    echo "  - Python packages: numpy, scipy, (scikit-learn, scikit-image, scikit-fuzzy, pandas, seaborn, jupyterlab, pillow)"
    echo ""
    echo "Virtual environment activated at .venv/"
    echo ""

    # Set library paths for dynamic linking
    export LD_LIBRARY_PATH="${pkgs.libGL}/lib:${pkgs.mesa}/lib:${pkgs.glfw}/lib:$LD_LIBRARY_PATH"
  '';

  # Environment variables for pkg-config to find libraries
  PKG_CONFIG_PATH = "${pkgs.glfw}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig";
}



# { pkgs ? import <nixpkgs> {} }:

# pkgs.mkShell {
#   buildInputs = with pkgs; [
#     # Python with pip for installing packages
#     python311
#     python311Packages.pip
#     python311Packages.virtualenv
#     python311Packages.packaging
    
#     # Core Python packages (pre-installed from Nix)
#     python311Packages.numpy
#     python311Packages.scipy
#     python311Packages.networkx
    
#     # Graphics/OpenGL libraries (project-specific)
#     glfw
#     glew
#     mesa
#     libGL
#     libGLU
#     glm
#     wayland    
#     # X11 libraries (project-specific)
#     xorg.libX11
#     xorg.libXrandr
#     xorg.libXi
#     xorg.libXcursor
#     xorg.libXinerama
    
#     # Build tool for pkg-config
#     pkg-config
#   ];

#   # Set up environment variables for OpenGL and graphics libraries
#   shellHook = ''
#     echo "🚀 OpenGL Python/C++ Development Environment"
#     echo "Python: $(python --version)"
#     echo "GCC: $(gcc --version | head -n1)"
#     echo ""
    
#     # Create and activate virtual environment if it doesn't exist
#     if [ ! -d .venv ]; then
#       echo "Creating Python virtual environment..."
#       python -m venv .venv
#     fi
    
#     source .venv/bin/activate
    
#     # Install scikit-fuzzy without matplotlib (avoid tkinter issues)
#     echo "Installing scikit-fuzzy..."
#     pip install --quiet scikit-fuzzy --no-deps 2>/dev/null || true
    
#     echo ""
#     echo "Available libraries:"
#     echo "  - GLFW (OpenGL window management)"
#     echo "  - Mesa (OpenGL implementation)"
#     echo "  - GLM (OpenGL Mathematics)"
#     echo "  - Python packages: numpy, scipy, scikit-fuzzy"
#     echo ""
#     echo "Virtual environment activated at .venv/"
#     echo ""
    
#     # Set library paths for dynamic linking
#     export LD_LIBRARY_PATH="${pkgs.libGL}/lib:${pkgs.mesa}/lib:${pkgs.glfw}/lib:$LD_LIBRARY_PATH"
#   '';

#   # Environment variables for pkg-config to find libraries
#   PKG_CONFIG_PATH = "${pkgs.glfw}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig";
# }

# { pkgs ? import <nixpkgs> {} }:

# pkgs.mkShell {
#   buildInputs = with pkgs; [
#     # Project-specific Python packages
#     python311Packages.numpy
#     python311Packages.scikit-fuzzy
    
#     # Graphics/OpenGL libraries (project-specific)
#     glfw
#     glew
#     mesa
#     libGL
#     libGLU
#     glm
    
#     # X11 libraries (project-specific)
#     xorg.libX11
#     xorg.libXrandr
#     xorg.libXi
#     xorg.libXcursor
#     xorg.libXinerama
    
#     # Build tool for pkg-config
#     pkg-config
#   ];

#   # Set up environment variables for OpenGL and graphics libraries
#   shellHook = ''
#     echo "🚀 OpenGL Python/C++ Development Environment"
#     echo "Python: $(python --version)"
#     echo "GCC: $(gcc --version | head -n1)"
#     echo ""
#     echo "Available libraries:"
#     echo "  - GLFW (OpenGL window management)"
#     echo "  - Mesa (OpenGL implementation)"
#     echo "  - GLM (OpenGL Mathematics)"
#     echo "  - Python packages: numpy, scikit-fuzzy"
#     echo ""
    
#     # Make sure Python can find the packages
#     export PYTHONPATH="${pkgs.python311Packages.numpy}/${pkgs.python311.sitePackages}:${pkgs.python311Packages.scikit-fuzzy}/${pkgs.python311.sitePackages}:$PYTHONPATH"
    
#     # Set library paths for dynamic linking
#     export LD_LIBRARY_PATH="${pkgs.libGL}/lib:${pkgs.mesa}/lib:${pkgs.glfw}/lib:$LD_LIBRARY_PATH"
#   '';

#   # Environment variables for pkg-config to find libraries
#   PKG_CONFIG_PATH = "${pkgs.glfw}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig";
# }