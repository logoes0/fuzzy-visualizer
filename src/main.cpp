#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Python.h> // Python embedding
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Quad vertices
float vertices[] = {
    0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
   -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f
};

std::string loadFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShaderFromFile(GLenum type, const std::string& path) {
    std::string source = loadFile(path);
    const char* src = source.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader error in " << path << ": " << infoLog << std::endl;
    }
    return shader;
}

GLuint createProgram(const std::string& vertPath, const std::string& fragPath) {
    GLuint vertexShader = compileShaderFromFile(GL_VERTEX_SHADER, vertPath);
    GLuint fragmentShader = compileShaderFromFile(GL_FRAGMENT_SHADER, fragPath);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

// Python embedded call
int getQualityEmbedded(PyObject* pFunc, float fps, float temp, float gpuLoad, float vramUsage, float motionIntensity) {
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cerr << "Python function not callable.\n";
        return 1;
    }

    PyObject* pArgs = PyTuple_Pack(5,
        PyFloat_FromDouble(fps),
        PyFloat_FromDouble(temp),
        PyFloat_FromDouble(gpuLoad),
        PyFloat_FromDouble(vramUsage),
        PyFloat_FromDouble(motionIntensity)
    );

    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    if (!pValue) {
        PyErr_Print();
        return 1; // default medium
    }

    int result = (int)PyLong_AsLong(pValue);
    Py_DECREF(pValue);
    return result;
}

int main() {
    // Init GLFW + OpenGL
    if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return -1; }
    GLFWwindow* window = glfwCreateWindow(800, 600, "Fuzzy Graphics Quality Demo", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to init GLEW\n"; return -1; }

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // OpenGL VAO/VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/sample.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { std::cerr << "Failed to load texture\n"; }
    stbi_image_free(data);

    // Shaders
    GLuint progHigh   = createProgram("shaders/quad.vert", "shaders/high.frag");
    GLuint progMedium = createProgram("shaders/quad.vert", "shaders/medium.frag");
    GLuint progLow    = createProgram("shaders/quad.vert", "shaders/low.frag");

    // Init Python
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");  // or 'src'
    PyObject* pName = PyUnicode_DecodeFSDefault("fuzzy_module"); // no .py
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load fuzzy_module.py\n";
        return -1;
    }
    PyObject* pFunc = PyObject_GetAttrString(pModule, "compute_quality");

    // Parameters (controlled via ImGui sliders)
    float fps = 75.0f;
    float temp = 55.0f;
    float gpuLoad = 40.0f;
    float vramUsage = 30.0f;
    float motionIntensity = 20.0f;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Graphics Parameters");
        ImGui::SliderFloat("FPS", &fps, 0.0f, 144.0f);
        ImGui::SliderFloat("GPU Temp", &temp, 20.0f, 100.0f);
        ImGui::SliderFloat("GPU Load %", &gpuLoad, 0.0f, 100.0f);
        ImGui::SliderFloat("VRAM Usage %", &vramUsage, 0.0f, 100.0f);
        ImGui::SliderFloat("Motion Intensity %", &motionIntensity, 0.0f, 100.0f);
        ImGui::End();

        // Call Python fuzzy logic
        int qualityIndex = getQualityEmbedded(pFunc, fps, temp, gpuLoad, vramUsage, motionIntensity);
        GLuint currentProgram = (qualityIndex == 0) ? progHigh : (qualityIndex == 1) ? progMedium : progLow;

        // Render scene
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(currentProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup Python
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();

    // Cleanup ImGui + OpenGL
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(progHigh);
    glDeleteProgram(progMedium);
    glDeleteProgram(progLow);

    glfwTerminate();
    return 0;
}
