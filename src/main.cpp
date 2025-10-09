#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(void)
{
    if (!glfwInit())
        return -1;
 
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "M45x3D", nullptr,nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (const int version = gladLoadGL(); version == 0)
        return -1;

    glClearColor(0.02f,0.02f,0.02f,0.0f);

    // Compile and activate shaders
    const GLuint vertex_shader = create_shader(GL_VERTEX_SHADER,parse_source("res/shaders/vs_basic.glsl").c_str());
    const GLuint geometry_shader = create_shader(GL_GEOMETRY_SHADER, parse_source("res/shaders/gs_basic.glsl").c_str());
    const GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, parse_source("res/shaders/fs_basic.glsl").c_str());

    const GLuint shader_program = glCreateProgram();
    
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, geometry_shader);
    glAttachShader(shader_program, fragment_shader);
    
    glLinkProgram(shader_program);
        
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(vertex_shader);

    glUseProgram(shader_program);

    // Create VBO with point coordinates
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat points[] = {
        -0.45f ,  0.45f , 4.0f ,
         0.45f ,  0.45f , 3.0f ,
        -0.45f , -0.45f , 4.0f ,
         0.45f , -0.45f , 8.0f ,
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Specify the layout of the vertex data
    const GLint pos_attrib = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, 0, 3 * sizeof(GLfloat), 0);

    const GLint sides_attrib = glGetAttribLocation(shader_program, "sides");
    glEnableVertexAttribArray(sides_attrib);
    glVertexAttribPointer(sides_attrib, 1, GL_FLOAT, 0, 3 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    float speed_X = 2.0f;
    float speed_Y = 0.15f;

    float Time = 0;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const float new_time = static_cast<float>(glfwGetTime());
        const float dt = new_time - Time;
        Time = new_time;
        
        speed_Y -= 10 * dt;
        
        if ( std::abs(points[0]) >= 1)
            speed_X *= -1.0f;
        if ( std::abs(points[1]) >= 1)
            speed_Y *= -0.99f;

        points[0] += speed_X * dt;
        points[1] += speed_Y * dt;
        
        points[0] = std::clamp(points[0],-1.0f,1.0f);
        points[1] = std::clamp(points[1],-1.0f,1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        
        glDrawArrays(GL_POINTS, 0,4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}