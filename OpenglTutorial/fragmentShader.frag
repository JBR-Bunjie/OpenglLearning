#version 330 core

in vec3 colorPer;

out vec4 color;

uniform vec4 ourColor;

void main()
{
    color = ourColor;
    color = vec4(colorPer.rgb, 1.0f);
}