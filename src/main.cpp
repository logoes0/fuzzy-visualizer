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

// Wireframe indices for cube edges
unsigned int wireframeIndices[] = {
    0, 1, 1, 2, 2, 3, 3, 0,  // front face
    4, 5, 5, 6, 6, 7, 7, 4,  // back face
    0, 4, 1, 5, 2, 6, 3, 7   // connecting edges
};

// Function to load shader source from file
std::string loadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to compile shader
GLuint compileShader(GLenum type, const std::string& source) {
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

// Function to create shader program
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
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

// Python integration function
int getQualityFromPython(PyObject* pFunc, float fps, float temp, float gpuLoad, float vramUsage, float motionIntensity) {
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

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Fuzzy 3D Cube Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 1200, 800);
    
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Create and bind VAO/VBO for cube
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Create wireframe EBO
    GLuint wireframeEBO;
    glGenBuffers(1, &wireframeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wireframeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireframeIndices), wireframeIndices, GL_STATIC_DRAW);
    
    // Create shader programs
    GLuint wireframeProgram = createShaderProgram("shaders/wireframe.vert", "shaders/wireframe.frag");
    GLuint flatProgram = createShaderProgram("shaders/flat.vert", "shaders/flat.frag");
    GLuint smoothProgram = createShaderProgram("shaders/smooth.vert", "shaders/smooth.frag");
    
    if (!wireframeProgram || !flatProgram || !smoothProgram) {
        std::cerr << "Failed to create shader programs" << std::endl;
        return -1;
    }
    
    // Initialize Python
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    
    PyObject* pName = PyUnicode_DecodeFSDefault("fuzzy_module");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load fuzzy_module.py" << std::endl;
        return -1;
    }
    
    PyObject* pFunc = PyObject_GetAttrString(pModule, "compute_quality");
    if (!pFunc) {
        std::cerr << "Failed to get compute_quality function" << std::endl;
        return -1;
    }
    
    // Parameters for fuzzy logic
    float fps = 60.0f;
    float temp = 65.0f;
    float gpuLoad = 50.0f;
    float vramUsage = 40.0f;
    float motionIntensity = 30.0f;
    
    // Camera variables
    float cameraDistance = 3.0f;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // ImGui controls
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
        ImGui::End();
        
        // Get quality from Python fuzzy logic
        int quality = getQualityFromPython(pFunc, fps, temp, gpuLoad, vramUsage, motionIntensity);
        
        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up view and projection matrices
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, cameraDistance),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
        
        // Model matrix with rotation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Render based on quality
        if (quality == 0) {
            // Low quality - Wireframe mode
            glUseProgram(wireframeProgram);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(2.0f);
            
            // Set uniforms
            glUniformMatrix4fv(glGetUniformLocation(wireframeProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(wireframeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(wireframeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, sizeof(wireframeIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            
        } else if (quality == 1) {
            // Medium quality - Flat shading
            glUseProgram(flatProgram);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            // Set uniforms
            glUniformMatrix4fv(glGetUniformLocation(flatProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(flatProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(flatProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            
            // Light position (fixed above, top-left corner)
            glm::vec3 lightPos = glm::vec3(-2.0f, 3.0f, 2.0f);
            glUniform3fv(glGetUniformLocation(flatProgram, "lightPos"), 1, glm::value_ptr(lightPos));
            glUniform3fv(glGetUniformLocation(flatProgram, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, cameraDistance)));
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
        } else {
            // High quality - Smooth shading with lighting
            glUseProgram(smoothProgram);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            // Set uniforms
            glUniformMatrix4fv(glGetUniformLocation(smoothProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(smoothProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(smoothProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            
            // Light position and properties
            glm::vec3 lightPos = glm::vec3(-2.0f, 3.0f, 2.0f);
            glUniform3fv(glGetUniformLocation(smoothProgram, "lightPos"), 1, glm::value_ptr(lightPos));
            glUniform3fv(glGetUniformLocation(smoothProgram, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, cameraDistance)));
            glUniform3fv(glGetUniformLocation(smoothProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
            glUniform3fv(glGetUniformLocation(smoothProgram, "ambientColor"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &wireframeEBO);
    glDeleteProgram(wireframeProgram);
    glDeleteProgram(flatProgram);
    glDeleteProgram(smoothProgram);
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}
