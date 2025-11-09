#include "../include/FuzzyCubeApp.h"

// Cube vertex data definitions
namespace CubeData {
    // Simple cube for low quality (24 triangles - visible faces only)
    float simpleCubeVertices[] = {
        // Front face (2 triangles)
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.0f,
        
        // Right face (2 triangles)
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        
        // Top face (2 triangles)
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        
        // Left face (2 triangles)
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f
    };

    // 3D Cube vertices with positions, normals, and colors
    float cubeVertices[] = {
        // positions          // normals           // colors
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f
    };

    // Screen quad vertices for post-processing
    float screenQuadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
}

// ShaderManager implementation
std::string ShaderManager::loadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint ShaderManager::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ShaderManager::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        return 0;
    }
    
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

// FramebufferManager implementation
FramebufferManager::FramebufferManager() : framebuffer(0), textureColorbuffer(0), rbo(0) {}

bool FramebufferManager::initialize(int width, int height) {
    // Create framebuffer for off-screen rendering
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    // Create color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    // Create renderbuffer for depth and stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void FramebufferManager::resize(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void FramebufferManager::bind() { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); }
void FramebufferManager::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
GLuint FramebufferManager::getTexture() const { return textureColorbuffer; }

void FramebufferManager::cleanup() {
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &rbo);
}

// CubeRenderer implementation
bool CubeRenderer::initialize() {
    std::cout << "[DEBUG CubeRenderer] Starting cube renderer initialization..." << std::endl;
    
    // Create and bind VAO/VBO for full cube
    std::cout << "[DEBUG CubeRenderer] Creating full cube VAO/VBO..." << std::endl;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    
    std::cout << "[DEBUG CubeRenderer] Binding full cube..." << std::endl;
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    
    std::cout << "[DEBUG CubeRenderer] Uploading full cube data (size: " << sizeof(CubeData::cubeVertices) << ")..." << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeData::cubeVertices), CubeData::cubeVertices, GL_STATIC_DRAW);
    
    std::cout << "[DEBUG CubeRenderer] Setting up full cube attributes..." << std::endl;
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Create and bind VAO/VBO for simple cube (low quality)
    std::cout << "[DEBUG CubeRenderer] Creating simple cube VAO/VBO..." << std::endl;
    glGenVertexArrays(1, &simpleCubeVAO);
    glGenBuffers(1, &simpleCubeVBO);
    
    std::cout << "[DEBUG CubeRenderer] Binding simple cube..." << std::endl;
    glBindVertexArray(simpleCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, simpleCubeVBO);
    
    std::cout << "[DEBUG CubeRenderer] Uploading simple cube data (size: " << sizeof(CubeData::simpleCubeVertices) << ")..." << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeData::simpleCubeVertices), CubeData::simpleCubeVertices, GL_STATIC_DRAW);
    
    std::cout << "[DEBUG CubeRenderer] Setting up simple cube attributes..." << std::endl;
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Create and bind VAO/VBO for screen quad
    std::cout << "[DEBUG CubeRenderer] Creating screen quad VAO/VBO..." << std::endl;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    std::cout << "[DEBUG CubeRenderer] Binding screen quad..." << std::endl;
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    
    std::cout << "[DEBUG CubeRenderer] Uploading screen quad data (size: " << sizeof(CubeData::screenQuadVertices) << ")..." << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeData::screenQuadVertices), CubeData::screenQuadVertices, GL_STATIC_DRAW);
    
    std::cout << "[DEBUG CubeRenderer] Setting up screen quad attributes..." << std::endl;
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    std::cout << "[DEBUG CubeRenderer] Cube renderer initialization complete!" << std::endl;
    
    return true;
}

void CubeRenderer::renderCube(GLuint program, int triangleCount) {
    glUseProgram(program);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, triangleCount);
}

void CubeRenderer::renderSimpleCube(GLuint program) {
    glUseProgram(program);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(simpleCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);
}

void CubeRenderer::renderScreenQuad() {
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CubeRenderer::cleanup() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &simpleCubeVAO);
    glDeleteBuffers(1, &simpleCubeVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

// ImGuiManager implementation
bool ImGuiManager::initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    return true;
}

void ImGuiManager::renderUI(float& fps, float& temp, float& gpuLoad, float& vramUsage, 
                           float& motionIntensity, float& cameraDistance, 
                           float& rotationX, float& rotationY, int quality, bool isManualOverride) {
    ImGui::Begin("Fuzzy Logic Parameters");
    ImGui::SliderFloat("FPS", &fps, 0.0f, 120.0f);
    ImGui::SliderFloat("GPU Temperature", &temp, 20.0f, 100.0f);
    ImGui::SliderFloat("GPU Load %", &gpuLoad, 0.0f, 100.0f);
    ImGui::SliderFloat("VRAM Usage %", &vramUsage, 0.0f, 100.0f);
    ImGui::SliderFloat("Motion Intensity %", &motionIntensity, 0.0f, 100.0f);
    ImGui::Separator();
    ImGui::SliderFloat("Camera Distance", &cameraDistance, 2.0f, 10.0f);
    ImGui::SliderFloat("Rotation X", &rotationX, -180.0f, 180.0f);
    ImGui::SliderFloat("Rotation Y", &rotationY, -180.0f, 180.0f);
    
    ImGui::Separator();
    ImGui::Text("Quality Control:");
    ImGui::Text("Press 1=Low, 2=Medium, 3=High, 0=Auto");
    
    // Show current quality mode
    const char* qualityNames[] = {"Low", "Medium", "High"};
    ImGui::Text("Current Quality: %s", qualityNames[quality]);
    if (isManualOverride) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(MANUAL)");
    } else {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "(AUTO)");
    }
    
    ImGui::End();
}

void ImGuiManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// PythonManager implementation
bool PythonManager::initialize() {
    std::cout << "[DEBUG] Starting Python initialization..." << std::endl;
    
    // Check if Python is already initialized
    if (Py_IsInitialized()) {
        std::cerr << "Python is already initialized" << std::endl;
        return false;
    }
    
    std::cout << "[DEBUG] Calling Py_InitializeEx(0)..." << std::endl;
    // Initialize Python (use Py_InitializeEx(0) to avoid signal handler issues)
    // Don't set PYTHONHOME programmatically - let environment handle it
    Py_InitializeEx(0);
    
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python" << std::endl;
        return false;
    }
    
    std::cout << "[DEBUG] Python initialized successfully" << std::endl;
    std::cout << "[DEBUG] Setting up Python paths..." << std::endl;
    
    // Add current directory to Python path and ensure venv site-packages are accessible
    PyRun_SimpleString(
        "import sys\n"
        "import os\n"
        "sys.path.insert(0, '.')\n"
        "# Add venv site-packages if in venv\n"
        "if hasattr(sys, 'real_prefix') or (hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix):\n"
        "    venv_site_packages = os.path.join(sys.prefix, 'lib', 'python' + '.'.join(map(str, sys.version_info[:2])), 'site-packages')\n"
        "    if os.path.exists(venv_site_packages):\n"
        "        sys.path.insert(0, venv_site_packages)\n"
    );
    
    std::cout << "[DEBUG] Paths configured" << std::endl;
    std::cout << "[DEBUG] Importing fuzzy_module..." << std::endl;
    
    // Import the fuzzy module
    PyObject* pName = PyUnicode_DecodeFSDefault("fuzzy_module");
    if (!pName) {
        PyErr_Print();
        std::cerr << "Failed to create module name" << std::endl;
        return false;
    }
    
    std::cout << "[DEBUG] Starting module import (this may take a few seconds)..." << std::endl;
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load fuzzy_module.py" << std::endl;
        std::cerr << "Make sure fuzzy_module.py is in the current directory" << std::endl;
        std::cerr << "Current working directory: ";
        PyRun_SimpleString("import os; print(os.getcwd())");
        return false;
    }
    
    std::cout << "[DEBUG] Module imported successfully" << std::endl;
    std::cout << "[DEBUG] Getting compute_quality function..." << std::endl;
    
    // Get the compute_quality function
    pFunc = PyObject_GetAttrString(pModule, "compute_quality");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PyErr_Print();
        std::cerr << "Failed to load compute_quality function" << std::endl;
        Py_XDECREF(pModule);
        pModule = nullptr;
        return false;
    }
    
    std::cout << "[DEBUG] Python initialization complete!" << std::endl;
    return true;
}

int PythonManager::getQuality(float fps, float temp, float gpuLoad, float vramUsage, float motionIntensity) {
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cerr << "Python function not callable" << std::endl;
        return 1; // Default to medium quality
    }
    
    PyObject* pArgs = PyTuple_Pack(5,
        PyFloat_FromDouble(fps),
        PyFloat_FromDouble(temp),
        PyFloat_FromDouble(gpuLoad),
        PyFloat_FromDouble(vramUsage),
        PyFloat_FromDouble(motionIntensity)
    );
    
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    
    if (!pResult) {
        PyErr_Print();
        return 1; // Default to medium quality
    }
    
    int result = (int)PyLong_AsLong(pResult);
    Py_DECREF(pResult);
    return result;
}

void PythonManager::cleanup() {
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

// QualitySettings implementation
QualitySettings QualitySettings::getSettings(int quality, GLuint simpleProgram, GLuint mediumProgram, 
                                           GLuint highProgram, GLuint simpleVAO, GLuint fullVAO) {
    QualitySettings settings;
    
    if (quality == 0) {
        // Low quality: Reduce everything
        settings.renderWidth = 600;   // 50% resolution
        settings.renderHeight = 400;
        settings.cubeProgram = simpleProgram;  // No lighting calculations
        settings.cubeVAO = simpleVAO;          // 24 triangles (4 visible faces)
        settings.triangleCount = 24;
        settings.pixelSize = 32.0f;   // More pixelation
    } else if (quality == 1) {
        // Medium quality: Moderate settings
        settings.renderWidth = 900;   // 75% resolution
        settings.renderHeight = 600;
        settings.cubeProgram = mediumProgram;  // Basic lighting only
        settings.cubeVAO = fullVAO;            // Full geometry
        settings.triangleCount = 36;
        settings.pixelSize = 64.0f;   // Medium pixelation
    } else {
        // High quality: Full quality
        settings.renderWidth = 1200;  // 100% resolution
        settings.renderHeight = 800;
        settings.cubeProgram = highProgram;    // Full lighting
        settings.cubeVAO = fullVAO;            // Full geometry
        settings.triangleCount = 36;
        settings.pixelSize = 200.0f;  // Minimal pixelation
    }
    
    return settings;
}

// FuzzyCubeApp implementation
bool FuzzyCubeApp::initialize() {
    std::cout << "[DEBUG] Starting application initialization..." << std::endl;
    
    // Initialize Python FIRST, before any OpenGL/threading initialization
    // This avoids conflicts between Python's threading and OpenGL's threading
    std::cout << "[DEBUG] Initializing Python (before OpenGL)..." << std::endl;
    if (!pythonManager.initialize()) {
        std::cerr << "Failed to initialize Python" << std::endl;
        return false;
    }
    std::cout << "[DEBUG] Python initialized successfully" << std::endl;
    
    // Initialize GLFW
    std::cout << "[DEBUG] Initializing GLFW..." << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    std::cout << "[DEBUG] Creating window..." << std::endl;
    window = glfwCreateWindow(1200, 800, "Fuzzy 3D Cube Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // CRITICAL: Make context current BEFORE initializing GLEW
    std::cout << "[DEBUG] Making OpenGL context current..." << std::endl;
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW with experimental features
    std::cout << "[DEBUG] Initializing GLEW..." << std::endl;
    glewExperimental = GL_TRUE;  // Enable modern OpenGL features
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    
    // Clear any OpenGL error that glewInit might have caused (known issue)
    glGetError();
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 1200, 800);
    
    // Initialize components
    std::cout << "[DEBUG] Initializing ImGui..." << std::endl;
    if (!ImGuiManager::initialize(window)) return false;
    std::cout << "[DEBUG] Initializing cube renderer..." << std::endl;
    if (!cubeRenderer.initialize()) return false;
    std::cout << "[DEBUG] Initializing framebuffer..." << std::endl;
    if (!framebufferManager.initialize(1200, 800)) return false;
    
    // Create shader programs
    cubeSimpleProgram = ShaderManager::createShaderProgram("shaders/cube_simple.vert", "shaders/cube_simple.frag");
    cubeMediumProgram = ShaderManager::createShaderProgram("shaders/cube_medium.vert", "shaders/cube_medium.frag");
    cubeHighProgram = ShaderManager::createShaderProgram("shaders/cube.vert", "shaders/cube.frag");
    pixelateProgram = ShaderManager::createShaderProgram("shaders/pixelate.vert", "shaders/pixelate.frag");
    
    if (!cubeSimpleProgram || !cubeMediumProgram || !cubeHighProgram || !pixelateProgram) {
        std::cerr << "Failed to create shader programs" << std::endl;
        return false;
    }
    
    // Debug: Print shader program IDs
    std::cout << "Cube Simple program ID: " << cubeSimpleProgram << std::endl;
    std::cout << "Cube Medium program ID: " << cubeMediumProgram << std::endl;
    std::cout << "Cube High program ID: " << cubeHighProgram << std::endl;
    std::cout << "Pixelate program ID: " << pixelateProgram << std::endl;
    
    return true;
}

void FuzzyCubeApp::handleInput() {
    // Manual quality override with keyboard input (1=low, 2=medium, 3=high)
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        manualQuality = 0; // Low quality
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        manualQuality = 1; // Medium quality
    } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        manualQuality = 2; // High quality
    } else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        manualQuality = -1; // Reset to automatic fuzzy logic
    }
}

void FuzzyCubeApp::render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Get quality for UI display
    int quality = pythonManager.getQuality(fps, temp, gpuLoad, vramUsage, motionIntensity);
    if (manualQuality >= 0) {
        quality = manualQuality;
    }
    
    // Render ImGui UI first
    ImGuiManager::renderUI(fps, temp, gpuLoad, vramUsage, motionIntensity, 
                          cameraDistance, rotationX, rotationY, 
                          quality, manualQuality >= 0);
    
    // Get quality settings
    QualitySettings settings = QualitySettings::getSettings(quality, cubeSimpleProgram, 
                                                          cubeMediumProgram, cubeHighProgram, 
                                                          cubeRenderer.getSimpleVAO(), 
                                                          cubeRenderer.getFullVAO());
    
    // Debug: Print current settings
    std::cout << "Quality: " << quality << " | Resolution: " << settings.renderWidth << "x" << settings.renderHeight 
              << " | Triangles: " << settings.triangleCount << " | PixelSize: " << settings.pixelSize;
    if (manualQuality >= 0) {
        std::cout << " (MANUAL)";
    }
    std::cout << std::endl;
    
    // Resize framebuffer texture based on quality
    framebufferManager.resize(settings.renderWidth, settings.renderHeight);
    
    // First pass: Render cube to framebuffer at quality-appropriate resolution
    framebufferManager.bind();
    glViewport(0, 0, settings.renderWidth, settings.renderHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up view and projection matrices
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, cameraDistance),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)settings.renderWidth / (float)settings.renderHeight, 0.1f, 100.0f);
    
    // Model matrix with rotation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Render cube with quality-appropriate shader and geometry
    glUseProgram(settings.cubeProgram);
    
    // Set common uniforms
    glUniformMatrix4fv(glGetUniformLocation(settings.cubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(settings.cubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(settings.cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // Set lighting uniforms only for medium and high quality
    if (quality >= 1) {
        glm::vec3 lightPos = glm::vec3(-2.0f, 3.0f, 2.0f);
        glUniform3fv(glGetUniformLocation(settings.cubeProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(settings.cubeProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        
        // High quality gets additional uniforms
        if (quality == 2) {
            glUniform3fv(glGetUniformLocation(settings.cubeProgram, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, cameraDistance)));
            glUniform3fv(glGetUniformLocation(settings.cubeProgram, "ambientColor"), 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
        }
    }
    
    // Render cube with appropriate geometry
    if (quality == 0) {
        cubeRenderer.renderSimpleCube(settings.cubeProgram);
    } else {
        cubeRenderer.renderCube(settings.cubeProgram, settings.triangleCount);
    }
    
    // Second pass: Render fullscreen quad with pixelation shader
    framebufferManager.unbind();
    glViewport(0, 0, 1200, 800);  // Always render final output at full screen resolution
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Disable depth testing for quad rendering
    glDisable(GL_DEPTH_TEST);
    
    glUseProgram(pixelateProgram);
    
    // Bind the framebuffer texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferManager.getTexture());
    glUniform1i(glGetUniformLocation(pixelateProgram, "screenTexture"), 0);
    
    // Set pixelation uniform
    glUniform1f(glGetUniformLocation(pixelateProgram, "pixelSize"), settings.pixelSize);
    
    // Render fullscreen quad
    cubeRenderer.renderScreenQuad();
    
    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FuzzyCubeApp::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        handleInput();
        render();
        
        glfwSwapBuffers(window);
    }
}

void FuzzyCubeApp::cleanup() {
    pythonManager.cleanup();
    cubeRenderer.cleanup();
    framebufferManager.cleanup();
    ImGuiManager::shutdown();
    
    glDeleteProgram(cubeSimpleProgram);
    glDeleteProgram(cubeMediumProgram);
    glDeleteProgram(cubeHighProgram);
    glDeleteProgram(pixelateProgram);
    
    glfwTerminate();
}
