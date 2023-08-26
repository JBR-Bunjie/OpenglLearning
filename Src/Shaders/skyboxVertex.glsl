#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

//uniform mat4 view;
//uniform mat4 proj;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 proj;
};

void main() {
    TexCoords = aPos;
    vec4 pos = proj * vec4(mat3x3(view) * aPos, 1.0);
    gl_Position = pos.xyww;
}