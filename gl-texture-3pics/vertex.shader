#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    v_texCoords = a_texCoord;
}