#version 330 core

layout (location = 0) in vec3 aPos;

// gl_Position是内置变量
void main()
{
     // 注意我们如何把一个vec3作为vec4的构造器的参数
    gl_Position = vec4(aPos, 1.0);
}