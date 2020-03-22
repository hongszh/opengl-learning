#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

// settings
const unsigned int SCR_WIDTH  = 500;
const unsigned int SCR_HEIGHT = 500;

const char *vertexShaderSource =
    "#version 450 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 450 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

int g_vertices_number;

void draw() {
    //设置清屏色
    glClear(GL_COLOR_BUFFER_BIT);

    // //设置颜色，红色
    glColor3f(1.0f, 0.0f, 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, g_vertices_number);

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);

    glutCreateWindow(argv[0]);

    if (glewInit()) {
        std::cerr << "Unable to initialize GLEW... Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // build and compile our shader program
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 下面几种顶点定义都可以
    // GLfloat  vertices[][2] = {
    //     { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
    //     {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
    // };

    // set up vertex data
    // glVertexAttribPointer的第二个参数得是3，顶点是3个分量
    // GLfloat vertices[][3] = {
    //     {-0.5f, -0.5f, 0.0f}, { 0.5f, -0.5f, 0.0f}, {-0.5f,  0.5f, 0.0f}, // Triangle 1
    //     { 0.5f, -0.5f, 0.0f}, { 0.5f,  0.5f, 0.0f}, {-0.5f,  0.5f, 0.0f}  // Triangle 2
    // };

    GLfloat vertices[][2] = {
        {-0.5f, -0.5f}, {0.5f, -0.5f}, {-0.5f, 0.5f}, // Triangle 1
        { 0.5f, -0.5f}, {0.5f,  0.5f}, {-0.5f, 0.5f}  // Triangle 2
    };
    
    // 计算顶点数量
    g_vertices_number = sizeof(vertices) / sizeof(vertices[0]);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);    
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glutDisplayFunc(draw);
    glutMainLoop();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
