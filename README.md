# Fuzzy 3D Cube Renderer

A C++ application that renders a 3D cube with dynamic quality settings
based on fuzzy logic. The renderer automatically switches between
wireframe, flat shading, and smooth shading based on system performance
metrics.

## Features

-   **3D Cube Rendering**: Renders a 3D cube using modern OpenGL.
-   **Dynamic Quality Switching**: Automatically adjusts rendering
    quality based on fuzzy logic.
-   **Interactive Controls**: ImGui interface for adjusting fuzzy logic
    parameters and camera controls.
-   **Python Integration**: Uses the Python C API to call a
    `scikit-fuzzy` logic module in real-time.

## Getting Started

There are two ways to set up the project. The recommended method uses
Nix to provide a fully reproducible environment automatically.

------------------------------------------------------------------------

### 🚀 With Nix (Recommended)

This method automatically provides all the correct versions of the
required system libraries and Python packages.

1.  **Install Nix** on your system if you don't have it already.
2.  **Clone the repository** and fetch the required ImGui submodule:
    `bash     git clone <repository_url>     cd fuzzy-visualizer     git submodule update --init --recursive`
3.  **Activate the Nix environment**. This command reads the `shell.nix`
    file and installs all dependencies into a temporary shell.
    `bash     nix-shell`
4.  **Build and Run** the application using the provided script.
    `bash     ./run.sh`

------------------------------------------------------------------------

### 🛠️ Manual Setup (Without Nix)

This method requires you to install all dependencies on your system
manually.

1.  **Clone the repository** and fetch the required ImGui submodule:
    `bash     git clone <repository_url>     cd fuzzy-visualizer     git submodule update --init --recursive`
2.  **Install System Libraries**. The following command is for **Arch
    Linux**. For other distributions like Debian/Ubuntu, you'll need to
    find the equivalent packages (e.g., `libglfw3-dev`, `libglew-dev`,
    `libglm-dev`).
    `bash     sudo pacman -S glfw-x11 mesa python glm glew wayland`
3.  **Install Python Dependencies** using pip.
    `bash     pip install -r requirements.txt`
4.  **Build and Run** the application using the provided script.
    `bash     ./run.sh`

## Project Structure

fuzzy-visualizer/ ├── src/ \# C++ source code (.cpp) ├── include/ \# C++
header files (.h) ├── shaders/ \# GLSL shader files ├── vendor/ \#
Third-party libraries (e.g., ImGui) ├── fuzzy_module.py \# Python fuzzy
logic module ├── shell.nix \# Nix environment definition ├── run.sh \#
Build and run script └── requirements.txt \# Python dependencies for
manual setup

## Troubleshooting

### Common Issues

1.  **GLM not found**: Install GLM development headers. (For manual
    setup) `bash     sudo pacman -S glm`
2.  **Python headers missing**: Ensure the Python development package is
    installed. (For manual setup) `bash     sudo pacman -S python`
3.  **Fuzzy module import error**: Ensure Python dependencies are
    installed. (For manual setup)
    `bash     pip install -r requirements.txt`
4.  **Missing library on Nix**: If you get a "file not found" error
    during compilation inside `nix-shell`, a package is likely missing
    from the `buildInputs` in `shell.nix`.

## License

This project is open source. Please check individual vendor libraries
for their respective licenses.
