#pragma once
#ifndef SHADER_H
//#define SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath = nullptr, const char* fragmentPath = nullptr, const char* geometryPath = nullptr);

    // 使用/激活程序
    void use();

    // 程序结束，释放VRAM
    void close();

    // uniform工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, glm::vec2 value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
private:
    enum ShaderType {
        VertexShader,
        FragmentShader,
        GeometryShader,
        ComputeShader
    };
    
    void checkCompileErrors(unsigned int shader, std::string type);
    unsigned int compileShader(const char* path, ShaderType shaderType = VertexShader);
};
#endif