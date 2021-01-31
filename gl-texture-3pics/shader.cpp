#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "shader.h"

Shader::Shader(void) {
}


Shader::~Shader(void) {
}

// read a file and return as a string
string Shader::ReadFile(const char* path) { 
    string content;
    ifstream fileStream(path, ios::in);

    if (!fileStream.is_open()) { 
        cerr << "File could not be opened" << endl;
        return "";
    }

    string line = "";
    while (!fileStream.eof()) { 
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::LoadShader(const char* vertPath, const char* fragPath) { 
    //generate shader names
    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    cout << "   vertPath: \n" << vertPath << endl;
    cout << "   fragPath: \n" << fragPath << endl;
    //get shader src
    string vertShaderStr = ReadFile(vertPath);
    string fragShaderStr = ReadFile(fragPath);

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    cout << " to compile vertex shader" << endl;

    //compile vertex shader
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);
    cout << " compiled vertex shader" << endl;

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> vertShaderError(logLength);
    if (!result) {
        glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
        cout << &vertShaderError[0] << endl;
    }

    cout << " to compile fragment shader" << endl;
    //compile fragment shader
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    cout << " compiled fragment shader" << endl;

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> fragShaderError(logLength);
    if (!result) {
        glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
        cout << &fragShaderError[0] << endl;
    }

    cout << " to link program" << endl;
    //link the program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> programError(logLength > 1 ? logLength : 1);
    if (!result) {
        glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
        cout << &programError[0] << endl;
    }

    cout << " to delete shader" << endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}