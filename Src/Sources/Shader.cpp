#include "Src/Headers/Shader.h"

void Shader::use() {
    glUseProgram(ID);
}

void Shader::close() {
    glDeleteProgram(ID);
}


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    // 着色器程序
    unsigned int vertex, fragment, geometry;
    ID = glCreateProgram();
    if (vertexPath != nullptr) {
        vertex = compileShader(vertexPath, VertexShader);
        glAttachShader(ID, vertex);
        glDeleteShader(vertex);
    }

    if (fragmentPath != nullptr) {
        fragment = compileShader(fragmentPath, FragmentShader);
        glAttachShader(ID, fragment);
        glDeleteShader(fragment);
    }

    if (geometryPath != nullptr) {
        geometry = compileShader(geometryPath, GeometryShader);
        glAttachShader(ID, geometry);
        glDeleteShader(geometry);
    }
    
    glLinkProgram(ID);
    checkCompileErrors(fragment, "PROGRAM");
}

unsigned int Shader::compileShader(const char* path, ShaderType shaderType) {
    std::string rawCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // 打开文件
        shaderFile.open(path);
        std::stringstream shaderStream;
        // 读取文件的缓冲内容到数据流中
        shaderStream << shaderFile.rdbuf();
        // 关闭文件处理器
        shaderFile.close();
        // 转换数据流到string
        rawCode = shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* shaderCode = rawCode.c_str();
    
    unsigned int shaderPointer;
    string shaderTypeString;

    if (shaderType == 0) {
        shaderPointer = glCreateShader(GL_VERTEX_SHADER);
        shaderTypeString = "VERTEX";
    } else if (shaderType == 1) {
        shaderPointer = glCreateShader(GL_FRAGMENT_SHADER);
        shaderTypeString = "FRAGMENT";
    } else if (shaderType == 2) {
        shaderPointer = glCreateShader(GL_GEOMETRY_SHADER);
        shaderTypeString = "GEOMETRY";
    } else if (shaderType == 3) {
        shaderPointer = glCreateShader(GL_COMPUTE_SHADER);
        shaderTypeString = "COMPUTE";
    }
    glShaderSource(shaderPointer, 1, &shaderCode, NULL);
    glCompileShader(shaderPointer);
    checkCompileErrors(shaderPointer, shaderTypeString);
    
    return shaderPointer;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}