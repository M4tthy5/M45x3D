#pragma once
#include <glad/glad.h>

// Shader creation helper
inline GLuint create_shader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}