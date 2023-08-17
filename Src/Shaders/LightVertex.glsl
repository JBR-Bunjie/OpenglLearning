#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0
//layout (location = 1) in vec2 aTexcoord; // 纹理坐标变量的属性位置值为2

out vec2 Texcoord; // 向片段着色器输出一个纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//uniform mat4 viewportTrans;

void main() {
    //    gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    gl_Position = proj * view * model * vec4(aPos, 1.0);
//    Texcoord = aTexcoord;
}