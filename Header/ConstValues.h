#pragma once
#ifndef ConstValues

const int initialWindowWidth = 800;
const int initialWindowHeight = 600;
const int initialViewportWidth = 800;
const int initialViewportHeight = 600;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char* vertexShaderSource = "Shader/vertex.glsl";
const char* fragmentShaderSource = "Shader/fragment.glsl";

#endif