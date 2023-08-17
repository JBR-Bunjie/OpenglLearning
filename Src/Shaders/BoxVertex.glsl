#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out vec3 Normal;
out vec3 worldPos;
out vec2 Texcoord; // 向片段着色器输出一个纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//uniform mat4 viewportTrans;

void main() {
    vec4 worldPosV4 = model * vec4(aPos, 1.0);
    worldPos = worldPosV4.xyz;
    gl_Position = proj * view * worldPosV4;

    Texcoord = aTexcoord;

//    Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}