#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

GLFWwindow* window;

int SCR_WIDTH  = 400;
int SCR_HEIGHT = 300;

int initGL() {
    // 初始化glfw库和glew
    // glfw: is an Open Source, multi-platform library for OpenGL, 
    // OpenGL ES and Vulkan development on the desktop. It provides
    // a simple API for creating windows, contexts and surfaces, 
    // receiving input and events.
    // glew: The OpenGL Extension Wrangler Library

    if (!glfwInit()) {
        return -1;
    }
    cout << "initialize glfw\n";
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        cout << "glfwTerminate" << endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glfwMakeContextCurrent 后面必须调用glewInit, 
    // 不然会segmentation-fault，细节没有研究
    GLenum err = glewInit(); 
    if( err != GLEW_OK ) {  
        cout << glewGetErrorString( err ) << endl;
        return  -1;  
    }

    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "usage: ./gl-texture image\n";
        return 0;
    }
    string imgFile = argv[1];

    if (initGL() != 0) {
        cout << "init GLfw failed!\n";
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    /* 下面这种情况，验证了下使用vbo没有vao的效果，实际上是有vbo和没有是一样的 */

    float vertices[] = {
        -1.0f,  1.0f, 1.0,  0,  0, //0左上
         1.0f,  1.0f, 1.0,  1,  0, //1右上
        -1.0f, -1.0f, 1.0,  0,  1, //2左下
         1.0f, -1.0f, 1.0,  1,  1, //3右下
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
    glEnableVertexAttribArray(1);

    // load shader
    cout << "load shader\n";
    Shader shader;
    GLuint txtId;
    GLuint program;
    {
        program = shader.LoadShader("../vertex.shader", "../fragment.shader");
        glUseProgram(program);

        glGenTextures(1, &txtId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, txtId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLint textureLoc = glGetUniformLocation(program, "s_texture");
        glUniform1i(textureLoc, 0);

        // 读取图片
        int width, height, channel;
        GLubyte *data = stbi_load(imgFile.c_str(), &width, &height, &channel, 0);
        if (data)
        {
            cout << "load image: " << imgFile.c_str() << " " << width << "x" << height << " done.\n";
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }

    // draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);

    return 0;
}
