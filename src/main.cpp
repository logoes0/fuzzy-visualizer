#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

float vertices[] = {
    // positions   // colors       // tex coords
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

int main() {
    if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return -1; }
    GLFWwindow* window = glfwCreateWindow(800, 600, "Fuzzy Graphics Quality Demo", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to init GLEW\n"; return -1; }

    // Create VAO/VBO
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

    // Load texture
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

    // Load shaders
    GLuint progHigh   = createProgram("shaders/quad.vert", "shaders/high.frag");
    GLuint progMedium = createProgram("shaders/quad.vert", "shaders/medium.frag");
    GLuint progLow    = createProgram("shaders/quad.vert", "shaders/low.frag");

    GLuint currentProgram = progHigh;

    // Keyboard input for quality change
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int sc, int action, int mods) {
        if (action == GLFW_PRESS) {
            GLuint* prog = (GLuint*)glfwGetWindowUserPointer(win);
            if (key == GLFW_KEY_1) *prog = 0; // high
            if (key == GLFW_KEY_2) *prog = 1; // medium
            if (key == GLFW_KEY_3) *prog = 2; // low
        }
    });
    int qualityIndex = 0;
    glfwSetWindowUserPointer(window, &qualityIndex);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (qualityIndex == 0) currentProgram = progHigh;
        if (qualityIndex == 1) currentProgram = progMedium;
        if (qualityIndex == 2) currentProgram = progLow;

        glUseProgram(currentProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(progHigh);
    glDeleteProgram(progMedium);
    glDeleteProgram(progLow);
    glfwTerminate();
    return 0;
}
