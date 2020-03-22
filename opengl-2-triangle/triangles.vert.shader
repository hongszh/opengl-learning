#version 450 core
 
layout(location = 0) in vec4 vPosition;

// gl_Position是内置变量
void main()
{
    gl_Position = vPosition;
}