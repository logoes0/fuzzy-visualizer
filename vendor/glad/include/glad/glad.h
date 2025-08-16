#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL header already included, remove this include, glad already provides it
#endif

#define __gl_h_

#include <KHR/khrplatform.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int GLenum;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

/* OpenGL 3.3 Core Profile */
#define GL_DEPTH_TEST 0x0B71
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_FRONT_AND_BACK 0x0408
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_TRIANGLES 0x0004
#define GL_LINES 0x0001
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_STATIC_DRAW 0x88E4
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

/* Shader types */
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30

/* Shader parameters */
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

/* Vertex attributes */
#define GL_FLOAT 0x1406

/* OpenGL functions */
GLAPI void APIENTRY glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY glClear(GLbitfield mask);
GLAPI void APIENTRY glEnable(GLenum cap);
GLAPI void APIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glPolygonMode(GLenum face, GLenum mode);
GLAPI void APIENTRY glLineWidth(GLfloat width);

/* Shader functions */
GLAPI GLuint APIENTRY glCreateShader(GLenum type);
GLAPI void APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
GLAPI void APIENTRY glCompileShader(GLuint shader);
GLAPI void APIENTRY glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
GLAPI void APIENTRY glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLAPI void APIENTRY glDeleteShader(GLuint shader);

/* Program functions */
GLAPI GLuint APIENTRY glCreateProgram(void);
GLAPI void APIENTRY glAttachShader(GLuint program, GLuint shader);
GLAPI void APIENTRY glLinkProgram(GLuint program);
GLAPI void APIENTRY glGetProgramiv(GLuint program, GLenum pname, GLint* params);
GLAPI void APIENTRY glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLAPI void APIENTRY glDeleteProgram(GLuint program);
GLAPI void APIENTRY glUseProgram(GLuint program);

/* Buffer functions */
GLAPI void APIENTRY glGenVertexArrays(GLsizei n, GLuint* arrays);
GLAPI void APIENTRY glGenBuffers(GLsizei n, GLuint* buffers);
GLAPI void APIENTRY glBindVertexArray(GLuint array);
GLAPI void APIENTRY glBindBuffer(GLenum target, GLuint buffer);
GLAPI void APIENTRY glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
GLAPI void APIENTRY glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
GLAPI void APIENTRY glEnableVertexAttribArray(GLuint index);
GLAPI void APIENTRY glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
GLAPI void APIENTRY glDeleteBuffers(GLsizei n, const GLuint* buffers);

/* Drawing functions */
GLAPI void APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count);
GLAPI void APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);

/* Uniform functions */
GLAPI GLint APIENTRY glGetUniformLocation(GLuint program, const GLchar* name);
GLAPI void APIENTRY glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLAPI void APIENTRY glUniform3fv(GLint location, GLsizei count, const GLfloat* value);

/* String query */
GLAPI const GLubyte* APIENTRY glGetString(GLenum name);
#define GL_VERSION 0x1F02

#ifdef __cplusplus
}
#endif

#endif
