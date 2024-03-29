﻿#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 worldPos;
out vec3 worldNormal;

uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 proj;
};

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0);
//    gl_Position = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
//    worldPos = vec3(model * vec4(aPos, 1.0));
    worldPos = aPos;
    worldNormal = mat3(transpose(inverse(model))) * aNormal;
}