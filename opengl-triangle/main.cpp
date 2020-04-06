
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "shader.h"

// 参考:
// https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/

void processInput(GLFWwindow *window);

int main(int argc, char **argv) {

    // glxinfo | grep OpenGL, 看到我shader versioon 是4.5，所以
    // export MESA_GL_VERSION_OVERRIDE=4.5

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }
    cout << "initialize glfw" << endl;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    cout << "glfwCreateWindow" << endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    cout << "glfwMakeContextCurrent" << endl;

    // glfwMakeContextCurrent 后面必须调用glewInit
    // 不然会segmentation-fault
    GLenum err = glewInit(); 
    if( err != GLEW_OK ) {  
        cout << glewGetErrorString( err ) << endl;
        return  -1;  
    }
    cout << "glewInit" << endl;

    // -------------准备数据-----------
    float vertices[] = {
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f   // top 
    };


    // 生成缓存对象
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 生成顶点数组对象
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // 加载shader
    Shader shader;
    GLuint program = shader.LoadShader("../vert.shader", "../frag.shader");
    glUseProgram(program);
    cout << "Loaded Shader." << endl;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 记得激活着色器
        glUseProgram(program);

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColor = glGetUniformLocation(program, "ourColor");
        glUniform4f(vertexColor, 0.0f, greenValue, 0.0f, 1.0f);

        // 绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
