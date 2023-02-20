#include "BasicLight.h"

BasicLight::BasicLight(
    glm::vec3 _lightPos,
    glm::vec3 _lightAmbient,
    glm::vec3 _lightDiffuse,
    glm::vec3 _lightSpecular):

    lightPos(_lightPos),
    lightAmbient(_lightAmbient),
    lightDiffuse(_lightDiffuse),
    lightSpecular(_lightSpecular) {
}


BasicLight::~BasicLight() {
}

void BasicLight::updateDirection() {
}

void BasicLight::setProgram(GLuint _program, const GLchar* _lightName) {
    program = _program;
    lightName = _lightName;
}


void BasicLight::applyLightColorProperties() {
    glUniform3f(glGetUniformLocation(program, (lightName + ambientPropertyName).c_str()), lightAmbient.x,
                lightAmbient.y, lightAmbient.z);
    glUniform3f(glGetUniformLocation(program, (lightName + diffusePropertyName).c_str()), lightDiffuse.x,
                lightDiffuse.y, lightDiffuse.z);
    glUniform3f(glGetUniformLocation(program, (lightName + specularPropertyName).c_str()), lightSpecular.x,
                lightSpecular.y, lightSpecular.z);
    
}

void BasicLight::applyLightPosProperties() {
    glUniform3f(glGetUniformLocation(program, lightPosName.c_str()), lightPos.x, lightPos.y, lightPos.z);
}
