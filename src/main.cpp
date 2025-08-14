#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

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

// ---------- FUZZY LOGIC ------------
int fuzzyQuality(float fps, float temp, float gpuLoad) {
    // Membership thresholds (simple)
    bool fpsHigh = fps >= 70;
    bool fpsMedium = fps >= 50 && fps < 70;
    bool fpsLow = fps < 50;

    bool tempLow = temp < 60;
    bool tempMedium = temp >= 60 && temp < 80;
    bool tempHigh = temp >= 80;

    bool loadLow = gpuLoad < 50;
    bool loadHigh = gpuLoad >= 50;

    // Rule-based fuzzy decisions
    if (fpsHigh && tempLow && loadLow) return 0; // High quality
    if (fpsLow || tempHigh) return 2; // Low quality
    return 1; // Medium quality
}
// -----------------------------------

int main() {
    srand(time(0));

    if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return -1; }
    GLFWwindow* window = glfwCreateWindow(800, 600, "Fuzzy Graphics Quality Demo", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to init GLEW\n"; return -1; }

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

    GLuint progHigh   = createProgram("shaders/quad.vert", "shaders/high.frag");
    GLuint progMedium = createProgram("shaders/quad.vert", "shaders/medium.frag");
    GLuint progLow    = createProgram("shaders/quad.vert", "shaders/low.frag");

    float fps = 75.0f;
    float temp = 55.0f;
    float gpuLoad = 40.0f;
    double lastUpdate = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        if (currentTime - lastUpdate >= 1.0) {
            // Simulate changing conditions every second
            fps = 40 + rand() % 50; // 40-89
            temp += (rand() % 5 - 2); // +/- 2 deg
            gpuLoad = rand() % 101; // 0-100
            if (temp < 40) temp = 40;
            if (temp > 90) temp = 90;

            std::cout << "FPS: " << fps << " Temp: " << temp << "C Load: " << gpuLoad << "%\n";
            lastUpdate = currentTime;
        }

        int qualityIndex = fuzzyQuality(fps, temp, gpuLoad);
        GLuint currentProgram = (qualityIndex == 0) ? progHigh : (qualityIndex == 1) ? progMedium : progLow;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
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
