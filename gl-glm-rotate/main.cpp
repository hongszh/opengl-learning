
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <iostream>

using namespace std;

GLFWwindow* window;

// settings
int SCR_WIDTH  = 400;
int SCR_HEIGHT = 300;

int initGLfw() {
    // 初始化glfw库和glew
    /* Initialize the glfw library */
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

    if (initGLfw() != 0) {
        cout << "init GLfw failed!\n";
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    // 顶点坐标和上面的定义一样，使用VBO来做
    float vertices[] = {
        -1.0f,  1.0f, 1.0, //0左上 
         1.0f,  1.0f, 1.0, //1右上
        -1.0f, -1.0f, 1.0, //2左下
         1.0f, -1.0f, 1.0  //3右下
    };

    float texCoords[] = {
         0,  0, //0左下放到左上
         1,  0, //1右下放到右上
         0,  1, //2左上放到左下
         1,  1  //3右上放到右下
    };

    GLuint vertexVbo, textureVbo, vao;
    glGenBuffers(1, &vertexVbo);
    glGenBuffers(1, &textureVbo);

    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    std::cout << glGetError() << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    std::cout << glGetError() << std::endl; 

    glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);


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
        GLint textureLoc = glGetUniformLocation(program, "sTexture");
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

    // glm的测试代码：把一个向量(1, 0, 0)位移(1, 1, 0)个单位
    // 使用的是0.9.9及以上版本，下面这行代码就需要改为带初始化参数:
    // glm::mat4 trans;
    glm::mat4 idttrans = glm::mat4(1.0f);
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    idttrans = glm::translate(idttrans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = idttrans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;

    // draw
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 创建变换矩阵，沿z轴逆时针旋转90度，然后缩放0.5倍，使它变成原来的一半大
        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
        // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        // 让图片缩放，位移，再随着时间绕z轴旋转
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 1.0));
        trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.f, 1.0f));

        GLint transformLoc = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &textureVbo);
    glfwTerminate();

    return 0;
}
