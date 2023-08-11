#version 330 core
in vec4 vertexColor;// 从顶点着色器传来的输入变量（名称相同、类型相同）

out vec4 FragColor;

uniform vec4 uniformTestColor;// 在Application中设定这个变量

void main() {
    FragColor = uniformTestColor;
}