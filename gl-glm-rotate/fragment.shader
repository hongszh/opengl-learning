#version 330 core

in vec2 vTexCoords;
out vec4 outColor;

uniform sampler2D sTexture;

void main()
{
    outColor = texture(sTexture, vTexCoords);
}