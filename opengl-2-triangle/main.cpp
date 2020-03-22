#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

#include <iostream>

#include "shader.h"

// 参考:
// https://476139183.github.io/2018/09/18/OpenGL-EL-%E4%BA%8C/
// https://www.photoneray.com/opengl-vao-vbo/
// http://www.opengl-tutorial.org/cn/beginners-tutorials/tutorial-2-the-first-triangle/

GLuint VBO;
GLuint VAO;

void Initialize() {

    // -------------准备数据-----------
    GLfloat vertices[][2] =
        {
            {-0.90, -0.90},
            {0.85,  -0.90},
            {-0.90,  0.85},

            {0.90,  -0.85},
            {0.90,   0.90},
            {-0.85,  0.90}};

    // 生成缓存对象
    glGenBuffers(1, &VBO);

    // 绑定缓存对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 填入数据，GL_ARRAY_BUFFER
    // 对于OpenGL他有特殊含义，那就是索引绘图必须使用这个缓存对象

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //------------设置顶点数据属性-------------
    // 生成顶点数组对象
    glGenVertexArrays(1, &VAO);

    // 绑定顶点数组对象
    glBindVertexArray(VAO);

    // 在渲染阶段, glVertexAttribPointer才确定数据作用类型（顶点位置、
    // float类型、从偏移量0处开始采集数据、2个float算一个采集步长等等。
    // 2的意思是顶点分量有两个，不是顶点的个数
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 加载shader
    Shader shader;
    GLuint program = shader.LoadShader("../triangles.vert.shader", "../triangles.frag.shader");
    glUseProgram(program);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 6是数组中顶点的数量，两个三角形6个顶点
    // 0s是从数组缓存中的第0个开始绘制
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();
}

int main(int argc, char **argv) {

    // 颜色没有生效或者下面这个error，需要export MESA_GL_VERSION_OVERRIDE环境变量
    // error: GLSL 3.30 is not supported. Supported versions are: 1.10, 1.20,
    // 1.30, 1.00 ES, and 3.00 ES.
    // glxinfo | grep OpenGL, 看到我shader versioon 是4.5，所以
    // export MESA_GL_VERSION_OVERRIDE=4.5

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutCreateWindow("sample");

    glewExperimental = true;
    if (glewInit()) {
        std::cerr << "Unable to initialize GLEW... Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Initialize();
    glutDisplayFunc(display);
    glutMainLoop();
}
