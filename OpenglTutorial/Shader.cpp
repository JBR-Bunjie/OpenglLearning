#include "Shader.h"

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath) {
    std::string vertShaderCodeData, fragShaderCodeData;
    std::ifstream vertShaderFile, fragShaderFile;

    vertShaderFile.exceptions(std::ifstream::badbit);
    fragShaderFile.exceptions(std::ifstream::badbit);
    try {
        // 打开文件
        vertShaderFile.open(vertexShaderPath);
        fragShaderFile.open(fragmentShaderPath);
        std::stringstream vertShaderStream, fragShaderStream;
        // 读取文件的缓冲内容到流中
        vertShaderStream << vertShaderFile.rdbuf();
        fragShaderStream << fragShaderFile.rdbuf();
        // 关闭文件
        vertShaderFile.close();
        fragShaderFile.close();
        // 转换流至GLchar数组
        vertShaderCodeData = vertShaderStream.str();
        fragShaderCodeData = fragShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* vertShaderCode = vertShaderCodeData.c_str();
    const GLchar* fragShaderCode = fragShaderCodeData.c_str();

    GLuint vertShader, fragShader;
    GLint success;
    GLchar infoLog[512];

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vertShaderCode, nullptr);
    glShaderSource(fragShader, 1, &fragShaderCode, nullptr);

    glCompileShader(vertShader);
    glCompileShader(fragShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };


    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertShader);
    glAttachShader(this->Program, fragShader);
    glLinkProgram(this->Program);

    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}


void Shader::Use() {
    glUseProgram(this->Program);
}