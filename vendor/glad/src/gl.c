#include <glad/glad.h>

/* OpenGL function pointers */
static void (*glClearColor_ptr)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
static void (*glClear_ptr)(GLbitfield) = NULL;
static void (*glEnable_ptr)(GLenum) = NULL;
static void (*glViewport_ptr)(GLint, GLint, GLsizei, GLsizei) = NULL;
static void (*glPolygonMode_ptr)(GLenum, GLenum) = NULL;
static void (*glLineWidth_ptr)(GLfloat) = NULL;
static GLuint (*glCreateShader_ptr)(GLenum) = NULL;
static void (*glShaderSource_ptr)(GLuint, GLsizei, const GLchar* const*, const GLint*) = NULL;
static void (*glCompileShader_ptr)(GLuint) = NULL;
static void (*glGetShaderiv_ptr)(GLuint, GLenum, GLint*) = NULL;
static void (*glGetShaderInfoLog_ptr)(GLuint, GLsizei, GLsizei*, GLchar*) = NULL;
static void (*glDeleteShader_ptr)(GLuint) = NULL;
static GLuint (*glCreateProgram_ptr)(void) = NULL;
static void (*glAttachShader_ptr)(GLuint, GLuint) = NULL;
static void (*glLinkProgram_ptr)(GLuint) = NULL;
static void (*glGetProgramiv_ptr)(GLuint, GLenum, GLint*) = NULL;
static void (*glGetProgramInfoLog_ptr)(GLuint, GLsizei, GLsizei*, GLchar*) = NULL;
static void (*glDeleteProgram_ptr)(GLuint) = NULL;
static void (*glUseProgram_ptr)(GLuint) = NULL;
static void (*glGenVertexArrays_ptr)(GLsizei, GLuint*) = NULL;
static void (*glGenBuffers_ptr)(GLsizei, GLuint*) = NULL;
static void (*glBindVertexArray_ptr)(GLuint) = NULL;
static void (*glBindBuffer_ptr)(GLenum, GLuint) = NULL;
static void (*glBufferData_ptr)(GLenum, GLsizeiptr, const void*, GLenum) = NULL;
static void (*glVertexAttribPointer_ptr)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*) = NULL;
static void (*glEnableVertexAttribArray_ptr)(GLuint) = NULL;
static void (*glDeleteVertexArrays_ptr)(GLsizei, const GLuint*) = NULL;
static void (*glDeleteBuffers_ptr)(GLsizei, const GLuint*) = NULL;
static void (*glDrawArrays_ptr)(GLenum, GLint, GLsizei) = NULL;
static void (*glDrawElements_ptr)(GLenum, GLsizei, GLenum, const void*) = NULL;
static GLint (*glGetUniformLocation_ptr)(GLuint, const GLchar*) = NULL;
static void (*glUniformMatrix4fv_ptr)(GLint, GLsizei, GLboolean, const GLfloat*) = NULL;
static void (*glUniform3fv_ptr)(GLint, GLsizei, const GLfloat*) = NULL;
static const GLubyte* (*glGetString_ptr)(GLenum) = NULL;

/* Function implementations */
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    if (glClearColor_ptr) glClearColor_ptr(red, green, blue, alpha);
}

void glClear(GLbitfield mask) {
    if (glClear_ptr) glClear_ptr(mask);
}

void glEnable(GLenum cap) {
    if (glEnable_ptr) glEnable_ptr(cap);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    if (glViewport_ptr) glViewport_ptr(x, y, width, height);
}

void glPolygonMode(GLenum face, GLenum mode) {
    if (glPolygonMode_ptr) glPolygonMode_ptr(face, mode);
}

void glLineWidth(GLfloat width) {
    if (glLineWidth_ptr) glLineWidth_ptr(width);
}

GLuint glCreateShader(GLenum type) {
    return glCreateShader_ptr ? glCreateShader_ptr(type) : 0;
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) {
    if (glShaderSource_ptr) glShaderSource_ptr(shader, count, string, length);
}

void glCompileShader(GLuint shader) {
    if (glCompileShader_ptr) glCompileShader_ptr(shader);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) {
    if (glGetShaderiv_ptr) glGetShaderiv_ptr(shader, pname, params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {
    if (glGetShaderInfoLog_ptr) glGetShaderInfoLog_ptr(shader, bufSize, length, infoLog);
}

void glDeleteShader(GLuint shader) {
    if (glDeleteShader_ptr) glDeleteShader_ptr(shader);
}

GLuint glCreateProgram(void) {
    return glCreateProgram_ptr ? glCreateProgram_ptr() : 0;
}

void glAttachShader(GLuint program, GLuint shader) {
    if (glAttachShader_ptr) glAttachShader_ptr(program, shader);
}

void glLinkProgram(GLuint program) {
    if (glLinkProgram_ptr) glLinkProgram_ptr(program);
}

void glGetProgramiv(GLuint program, GLenum pname, GLint* params) {
    if (glGetProgramiv_ptr) glGetProgramiv_ptr(program, pname, params);
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {
    if (glGetProgramInfoLog_ptr) glGetProgramInfoLog_ptr(program, bufSize, length, infoLog);
}

void glDeleteProgram(GLuint program) {
    if (glDeleteProgram_ptr) glDeleteProgram_ptr(program);
}

void glUseProgram(GLuint program) {
    if (glUseProgram_ptr) glUseProgram_ptr(program);
}

void glGenVertexArrays(GLsizei n, GLuint* arrays) {
    if (glGenVertexArrays_ptr) glGenVertexArrays_ptr(n, arrays);
}

void glGenBuffers(GLsizei n, GLuint* buffers) {
    if (glGenBuffers_ptr) glGenBuffers_ptr(n, buffers);
}

void glBindVertexArray(GLuint array) {
    if (glBindVertexArray_ptr) glBindVertexArray_ptr(array);
}

void glBindBuffer(GLenum target, GLuint buffer) {
    if (glBindBuffer_ptr) glBindBuffer_ptr(target, buffer);
}

void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
    if (glBufferData_ptr) glBufferData_ptr(target, size, data, usage);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
    if (glVertexAttribPointer_ptr) glVertexAttribPointer_ptr(index, size, type, normalized, stride, pointer);
}

void glEnableVertexAttribArray(GLuint index) {
    if (glEnableVertexAttribArray_ptr) glEnableVertexAttribArray_ptr(index);
}

void glDeleteVertexArrays(GLsizei n, const GLuint* arrays) {
    if (glDeleteVertexArrays_ptr) glDeleteVertexArrays_ptr(n, arrays);
}

void glDeleteBuffers(GLsizei n, const GLuint* buffers) {
    if (glDeleteBuffers_ptr) glDeleteBuffers_ptr(n, buffers);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    if (glDrawArrays_ptr) glDrawArrays_ptr(mode, first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if (glDrawElements_ptr) glDrawElements_ptr(mode, count, type, indices);
}

GLint glGetUniformLocation(GLuint program, const GLchar* name) {
    return glGetUniformLocation_ptr ? glGetUniformLocation_ptr(program, name) : -1;
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
    if (glUniformMatrix4fv_ptr) glUniformMatrix4fv_ptr(location, count, transpose, value);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat* value) {
    if (glUniform3fv_ptr) glUniform3fv_ptr(location, count, value);
}

const GLubyte* glGetString(GLenum name) {
    return glGetString_ptr ? glGetString_ptr(name) : NULL;
}

/* Load function pointers from OpenGL context */
int gladLoadGL(void) {
    /* This is a simplified loader - in a real implementation, you would use glfwGetProcAddress */
    /* For now, we'll assume the functions are available */
    return 1;
}
