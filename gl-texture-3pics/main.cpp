#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

GLFWwindow* window;

int SCR_WIDTH  = 400*2 + 300*16/9 + 10*4;
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

    // 使用一个VBO，glBufferSubData 缓存两个buffer到VBO，然后绘制texture
    // 参考：https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/07%20Advanced%20Data/


    // 定义三组不同的顶点，分别对应与三个vao，绘制三组图像
    // original
    float verticesOriginal[] = {
        -1.0f,  1.0f, 1.0, //0左上 
         1.0f,  1.0f, 1.0, //1右上
        -1.0f, -1.0f, 1.0, //2左下
         1.0f, -1.0f, 1.0  //3右下
    };

    // zoom out
    float verticesZoomOut[] = {
        -0.5f,  0.5f, 1.0, //0左上 
         0.5f,  0.5f, 1.0, //1右上
        -0.5f, -0.5f, 1.0, //2左下
         0.5f, -0.5f, 1.0  //3右下
    };


    // zoom in & crop
    float verticesZoomIn[] = {
        -1.4f,  1.4f, 1.0, //0左上 
         1.4f,  1.4f, 1.0, //1右上
        -1.4f, -1.4f, 1.0, //2左下
         1.4f, -1.4f, 1.0  //3右下
    };


    // 纹理坐标
    float texCoords[] = {
         0,  0, //0左下放到左上
         1,  0, //1右下放到右上
         0,  1, //2左上放到左下
         1,  1  //3右上放到右下
    };


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

    size_t vSize, tSize;
    GLuint vbo, vaoLeft, vaoSmall, vaoRight;

    // 生成绘制原始图像的vao
    vSize = sizeof(verticesOriginal);
    tSize = sizeof(texCoords);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vaoLeft);
	glBindVertexArray(vaoLeft);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vSize+tSize, nullptr, GL_STATIC_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, &verticesOriginal);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, tSize, &texCoords);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (GLvoid*)(sizeof(verticesOriginal)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 生成绘制缩小图像的vao
    vSize = sizeof(verticesZoomOut);
    tSize = sizeof(texCoords);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vaoSmall);
	glBindVertexArray(vaoSmall);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vSize+tSize, nullptr, GL_STATIC_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, &verticesZoomOut);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, tSize, &texCoords);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (GLvoid*)(sizeof(verticesZoomOut)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 生成绘制放大1.4倍图像的vao
    vSize = sizeof(verticesZoomIn);
    tSize = sizeof(texCoords);
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vaoRight);
	glBindVertexArray(vaoRight);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vSize+tSize, nullptr, GL_STATIC_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, &verticesZoomIn);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, tSize, &texCoords);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (GLvoid*)(sizeof(verticesZoomIn)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw left，set viewport aspect ratio to 400:300
        glViewport(10, 0, 400, 300);
        glBindVertexArray(vaoLeft);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);


        //draw small，set viewport aspect ratio to 400:300
        glViewport(420, 0, 400, 300);
        glBindVertexArray(vaoSmall);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);


        //draw right，set viewport aspect ratio to 533:300
        glViewport(830, 0, 300*16/9, 300);
        glBindVertexArray(vaoRight);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vaoLeft);
    glDeleteVertexArrays(1, &vaoSmall);
    glDeleteVertexArrays(1, &vaoRight);
    glDeleteBuffers(1, &vbo);

    return 0;
}
