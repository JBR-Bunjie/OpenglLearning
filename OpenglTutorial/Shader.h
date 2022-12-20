#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>

#include <sstream>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>

#pragma once
class Shader {
public:
	// program ID - 程序ID
	GLuint Program;
	// 构造器读取并构建着色器
	Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
	//
	void Use();

private:

};

#endif