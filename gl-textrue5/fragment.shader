#version 330 core

in vec2 v_texCoords;
out vec4 outColor;

uniform sampler2D s_texture;

void main()
{
    outColor = texture(s_texture, v_texCoords);
}