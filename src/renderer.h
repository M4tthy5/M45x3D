#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <fstream>
#include <sstream>

// Shader creation helper
inline GLuint create_shader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

inline std::string parse_source(const GLchar* file)
{
    std::ifstream stream(file);
    std::string type;
    int id = 0;
    std::stringstream ss[3];
    std::string line;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                id = 0;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                id = 1;
            }
            else if (line.find("geometry") != std::string::npos)
            {
                id = 2;
            }
        }else
        {
            ss[id] << line << "\n";
        }
    }
    return ss[id].str();
}
