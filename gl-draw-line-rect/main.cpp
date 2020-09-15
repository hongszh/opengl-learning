// refer to:
// https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/

// error:
// 通过gfxinfo看到我的glsl是4.5，在没有export的时候，可能输出是白的
// export MESA_GL_VERSION_OVERRIDE=4.5
// 安装cuda后就不存在这个问题了

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

// settings
int SCR_WIDTH  = 400;
int SCR_HEIGHT = 300;


void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "usage: ./gl-draw line[rect]\n";
        return 0;
    }

    int drawLine;
    if(argv[1] == string("line")) {
        drawLine = 1;
    } else if (argv[1] == string("rect")) {
        drawLine = 0;
    } else {
        cout << "usage: ./gl-draw line[rect]\n";
        return 0;
    }

    // 初始化glfw库和glew
    /* Initialize the glfw library */
    if (!glfwInit()) {
        return -1;
    }
    cout << "initialize glfw\n";
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        cout << "glfwTerminate" << endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glfwMakeContextCurrent 后面必须调用glewInit
    // 不然会segmentation-fault，原因不详
    GLenum err = glewInit(); 
    if( err != GLEW_OK ) {  
        cout << glewGetErrorString( err ) << endl;
        return  -1;  
    }

    // openGL编程从这里开始
    // 定义顶点数据(左上开始的顺时针顶点次序)
    float vertices0[] = {
        -0.5f,  0.5f, 1.0, //1左上
         0.5f,  0.5f, 1.0, //2右上
         0.5f, -0.5f, 1.0, //3右下
        -0.5f, -0.5f, 1.0, //4左下
    };

    // 定义顶点数据(正Z字形)
    float vertices1[] = {
        -0.5f,  0.5f, 1.0, //1左上 
         0.5f,  0.5f, 1.0, //2右上
        -0.5f, -0.5f, 1.0, //4左下
         0.5f, -0.5f, 1.0, //3右下
    };

    // 定义顶点数据(反Z字形)
    float vertices2[] = {
         0.5f,  0.5f, 1.0, //2右上
        -0.5f,  0.5f, 1.0, //1左上 
         0.5f, -0.5f, 1.0, //3右下
        -0.5f, -0.5f, 1.0, //4左下
    };

    // 用vertices0绘制的是顺序顶点的效果，不能绘制出矩形
    // 用vertices1和vertices2可以观察正反Z字形次序的顶点都可以绘制出矩形
    float *vertices = vertices0;

    // 生成顶点缓冲对象，使用glBufferData函数填充缓冲对象管理的内存
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*4, vertices, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    cout << "load shader\n";
    Shader shader;
    GLuint program = shader.LoadShader("../vertex.shader", "../fragment.shader");
    glUseProgram(program);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        if (drawLine) {
            glDrawArrays(GL_LINE_STRIP, 0, 4);
        } else {
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
