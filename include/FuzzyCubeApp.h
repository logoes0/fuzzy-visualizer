#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Python.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Global verbose flag for debug output
extern bool g_verbose;

// OpenGL debugging utilities
void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* userParam);
void checkGLError(const char* operation);

// Forward declarations
class CubeRenderer;
class ShaderManager;
class FramebufferManager;
class ImGuiManager;
class PythonManager;

// Cube vertex data namespace
namespace CubeData {
    extern float simpleCubeVertices[];
    extern float cubeVertices[];
    extern float screenQuadVertices[];
}

// Shader management class
class ShaderManager {
public:
    static std::string loadShaderSource(const std::string& filePath);
    static GLuint compileShader(GLenum type, const std::string& source);
    static GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
};

// Structure to hold FBO resources for a single quality level
struct QualityFBO {
    GLuint framebuffer;
    GLuint textureColorbuffer;
    GLuint rbo;
    int width, height;
};

// Framebuffer management class
class FramebufferManager {
private:
    QualityFBO fbos[3];  // One FBO per quality level (low=0, medium=1, high=2)
    int currentBoundQuality = -1;

public:
    FramebufferManager();
    bool initialize();  // Creates all 3 FBOs with appropriate resolutions
    void bind(int quality);  // Bind FBO for specific quality level
    void unbind();
    GLuint getTexture(int quality) const;
    int getWidth(int quality) const { return fbos[quality].width; }
    int getHeight(int quality) const { return fbos[quality].height; }
    void cleanup();
};

// Cube renderer class
class CubeRenderer {
private:
    GLuint cubeVAO, cubeVBO;
    GLuint simpleCubeVAO, simpleCubeVBO;
    GLuint quadVAO, quadVBO;

public:
    bool initialize();
    void renderCube(GLuint program, int triangleCount);
    void renderSimpleCube(GLuint program);
    void renderScreenQuad();
    void cleanup();
    GLuint getSimpleVAO() const { return simpleCubeVAO; }
    GLuint getFullVAO() const { return cubeVAO; }
};

// ImGui management class
class ImGuiManager {
public:
    static bool initialize(GLFWwindow* window);
    static void renderUI(float& fps, float& temp, float& gpuLoad, float& vramUsage, 
                        float& motionIntensity, float& cameraDistance, 
                        float& rotationX, float& rotationY, int quality, bool isManualOverride);
    static void shutdown();
};

// Python integration class
class PythonManager {
private:
    PyObject* pModule;
    PyObject* pFunc;

public:
    bool initialize();
    int getQuality(float fps, float temp, float gpuLoad, float vramUsage, float motionIntensity);
    void cleanup();
};

// Quality settings structure
struct QualitySettings {
    int renderWidth, renderHeight;
    GLuint cubeProgram;
    GLuint cubeVAO;
    int triangleCount;
    float pixelSize;
    
    static QualitySettings getSettings(int quality, GLuint simpleProgram, GLuint mediumProgram, 
                                     GLuint highProgram, GLuint simpleVAO, GLuint fullVAO);
};

// Main application class
class FuzzyCubeApp {
private:
    GLFWwindow* window;
    ShaderManager shaderManager;
    FramebufferManager framebufferManager;
    CubeRenderer cubeRenderer;
    ImGuiManager imguiManager;
    PythonManager pythonManager;
    
    // Shader programs
    GLuint cubeSimpleProgram, cubeMediumProgram, cubeHighProgram, pixelateProgram;
    
    // UI state
    float fps = 60.0f, temp = 50.0f, gpuLoad = 30.0f, vramUsage = 40.0f, motionIntensity = 20.0f;
    float cameraDistance = 3.0f, rotationX = 0.0f, rotationY = 0.0f;
    
    // Manual override state
    int manualQuality = -1; // -1 means use fuzzy logic

public:
    bool initialize();
    void handleInput();
    void render();
    void run();
    void cleanup();
};
