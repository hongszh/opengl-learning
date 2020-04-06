
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
         // 顶点位置          // 颜色
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top 
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // 加载shader
    Shader shader;
    GLuint program = shader.LoadShader("../vert.shader", "../frag.shader");
    glUseProgram(program);
    cout << "Loaded Shader." << endl;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
