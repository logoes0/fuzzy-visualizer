# Fuzzy 3D Cube Renderer

A C++ application that renders a 3D cube with dynamic quality settings based on fuzzy logic. The renderer automatically switches between wireframe, flat shading, and smooth shading based on system performance metrics.

## Features

- **3D Cube Rendering**: Renders a textured 3D cube with proper lighting
- **Dynamic Quality Switching**: Automatically adjusts rendering quality based on fuzzy logic
- **Three Rendering Modes**:
  - **Low Quality (0)**: Wireframe rendering for maximum performance
  - **Medium Quality (1)**: Flat shading with basic lighting
  - **High Quality (2)**: Smooth shading with advanced lighting and specular highlights
- **Interactive Controls**: ImGui interface for adjusting fuzzy logic parameters
- **Python Integration**: Uses Python C API to call fuzzy logic module
- **Real-time Performance**: GLFW + OpenGL for smooth real-time rendering

## Architecture

- **Frontend**: GLFW + OpenGL + ImGui
- **Backend**: Python fuzzy logic module using scikit-fuzzy
- **Rendering Pipeline**: Three shader programs for different quality levels
- **Performance Metrics**: FPS, GPU temperature, GPU load, VRAM usage, motion intensity

## Dependencies

### System Libraries (Arch Linux)
```bash
sudo pacman -S glfw-x11 glfw-wayland mesa libx11 libxrandr libxi python glm
```

### Python Dependencies
```bash
pip install -r requirements.txt
```

## Project Structure

```
fuzzy-visualizer/
├── src/
│   └── main.cpp              # Main C++ application
├── shaders/
│   ├── wireframe.vert        # Wireframe vertex shader
│   ├── wireframe.frag        # Wireframe fragment shader
│   ├── flat.vert            # Flat shading vertex shader
│   ├── flat.frag            # Flat shading fragment shader
│   ├── smooth.vert          # Smooth shading vertex shader
│   └── smooth.frag          # Smooth shading fragment shader
├── vendor/
│   ├── glad/                 # GLAD OpenGL loader
│   └── imgui/                # ImGui library
├── fuzzy_module.py           # Python fuzzy logic module
├── run.sh                    # Build and run script
└── requirements.txt          # Python dependencies
```

## Building and Running

### Quick Start
```bash
./run.sh
```

### Manual Build
```bash
mkdir -p build

g++ src/main.cpp \
    vendor/glad/src/gl.c \
    vendor/imgui/imgui.cpp \
    vendor/imgui/imgui_draw.cpp \
    vendor/imgui/imgui_tables.cpp \
    vendor/imgui/imgui_widgets.cpp \
    vendor/imgui/backends_local/imgui_impl_glfw.cpp \
    vendor/imgui/backends_local/imgui_impl_opengl3.cpp \
    -Iinclude \
    -Ivendor/glad/include \
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
    -o build/app
```

### Running the Application
```bash
./build/app
```

## Usage

1. **Launch the application** using the build script or manual build
2. **Adjust fuzzy logic parameters** using the ImGui interface:
   - FPS: Target frame rate (0-120)
   - GPU Temperature: Current GPU temperature (20-100°C)
   - GPU Load: Current GPU utilization (0-100%)
   - VRAM Usage: Video memory usage (0-100%)
   - Motion Intensity: Scene movement intensity (0-100%)
3. **Control camera** with the additional sliders:
   - Camera Distance: Distance from cube (2-10 units)
   - Rotation X/Y: Cube rotation angles (-180° to 180°)
4. **Observe quality changes** as the fuzzy logic automatically switches rendering modes

## Fuzzy Logic Rules

The Python module implements the following fuzzy logic rules:

- **High Quality**: High FPS + Cool GPU + Low Load + Low VRAM
- **Medium Quality**: Medium FPS OR Warm GPU OR High Load OR High VRAM
- **Low Quality**: Low FPS OR Hot GPU OR Fast Motion

## Technical Details

### Shader Programs
- **Wireframe**: Simple vertex/fragment shaders for line rendering
- **Flat**: Basic lighting with ambient and diffuse components
- **Smooth**: Advanced lighting with ambient, diffuse, specular, and attenuation

### OpenGL Features
- Vertex Buffer Objects (VBO)
- Vertex Array Objects (VAO)
- Element Buffer Objects (EBO) for wireframe
- Depth testing enabled
- Modern OpenGL 3.3 core profile

### Python Integration
- Uses Python C API for embedding
- Calls `fuzzy_module.compute_quality()` function
- Handles Python object lifecycle properly
- Graceful fallback on errors

## Troubleshooting

### Common Issues

1. **GLM not found**: Install GLM development headers
   ```bash
   sudo pacman -S glm
   ```

2. **Python headers missing**: Ensure Python development package is installed
   ```bash
   sudo pacman -S python
   ```

3. **OpenGL context creation failed**: Check graphics drivers and OpenGL support
   ```bash
   glxinfo | grep "OpenGL version"
   ```

4. **Fuzzy module import error**: Ensure Python dependencies are installed
   ```bash
   pip install -r requirements.txt
   ```

### Debug Information
The application provides detailed error messages for:
- Shader compilation errors
- OpenGL context creation failures
- Python module loading issues
- Shader program linking problems

## Performance Considerations

- **Wireframe mode**: Minimal GPU usage, suitable for low-end systems
- **Flat shading**: Moderate GPU usage, balanced performance/quality
- **Smooth shading**: Higher GPU usage, best visual quality
- **Real-time switching**: Fuzzy logic updates every frame for responsive quality changes

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test with different quality settings
5. Submit a pull request

## License

This project is open source. Please check individual vendor libraries for their respective licenses.