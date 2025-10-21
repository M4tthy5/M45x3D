#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 proj = glm::ortho(-2,2,-1,1);

int wx = 200 , wy = 200;

void OnWindowResize(GLFWwindow* window, int width, int height){
    wx = width; wy = height;
    glViewport(0, 0, width, height);
}

GLFWcursor* base_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
GLFWcursor* hand_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR );

int main(void)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(wx, wy, "M45x3D", nullptr,nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetCursor(window,base_cursor);
    glfwSetFramebufferSizeCallback(window,OnWindowResize);

    if (const int version = gladLoadGL(); version == 0)
        return -1;

    glClearColor(0.025f,0.0125f,0,0.6f);

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
        -0.45f ,  0.45f , 20.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLint pos_attrib = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, 0, 3 * sizeof(GLfloat), 0);

    const GLint sides_attrib = glGetAttribLocation(shader_program, "sides");
    glEnableVertexAttribArray(sides_attrib);
    glVertexAttribPointer(sides_attrib, 1, GL_FLOAT, 0, 3 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    float speed_X = 1.0f;
    float speed_Y = 1.0f;

    float Time = 0;
    unsigned int step = 0;
    float LastTime = 0;
    double m_x = 0, m_y = 0;

    std::cout << R"(M45x3D init)";

    while (!glfwWindowShouldClose(window))
    {
        glfwGetCursorPos(window, &m_x, &m_y);
        m_x = m_x/(0.5f*wx) - 1;
        m_y = 1 - m_y/(0.5f*wy);

        const float new_time = static_cast<float>(glfwGetTime());
        const float dt = new_time - Time;
        Time = new_time;
        step++;

        speed_Y -= speed_Y * 0.1f * dt;
        speed_X -= speed_X * 0.1f * dt;

        if (glfwGetMouseButton(window,0))
        {
            glfwSetCursor(window,hand_cursor);
            speed_X += (static_cast<float>(m_x) - points[0] - speed_X * 0.1f) * dt * 50;
            speed_Y += (static_cast<float>(m_y) - points[1] - speed_Y * 0.1f) * dt * 50;
        }
        else
        {
            glfwSetCursor(window,base_cursor);
            speed_Y -= 10 * dt;
        }

        if ( std::abs(points[0]) >= 0.9f){
            speed_X *= -1.0f;
            points[0] = std::clamp(points[0],-.9f,.9f);
        }
        if ( std::abs(points[1]) >= 0.9f){
            speed_Y *= -1.0f;
            points[1] = std::clamp(points[1],-.9f,.9f);
        }
        points[0] += speed_X * dt;
        points[1] += speed_Y * dt;

        glClear(GL_COLOR_BUFFER_BIT);

        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        glDrawArrays(GL_POINTS, 0,1);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(Time>(1+LastTime)){
            std::cout << "\a" + std::to_string(step) + "fps\n";
            step = 0;LastTime = Time;
        }
    }

    glDeleteProgram(shader_program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}
