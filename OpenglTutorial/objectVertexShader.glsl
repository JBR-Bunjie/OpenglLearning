#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec2 Texcoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main()
{
    gl_Position = project * view * model * transform * vec4(position.x, position.y, position.z, 1.0f);

    FragPos = vec3(model * transform * vec4(position, 1.0f));

    Normal = mat3(transpose(inverse(model * transform))) * normal;

    Texcoord = texcoord;
}