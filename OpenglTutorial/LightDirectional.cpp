#include "LightDirectional.h"
#include "BasicLight.h"

LightDirectional::LightDirectional(
    glm::vec3 _lightPos,
    glm::vec3 _lightAmbient,
    glm::vec3 _lightDiffuse,
    glm::vec3 _lightSpecular
) : BasicLight::BasicLight(
    _lightPos,
    _lightAmbient,
    _lightDiffuse,
    _lightSpecular
) {
    // lightDirection = _lightPos;
}

LightDirectional::~LightDirectional() {
    
}

void LightDirectional::updateColor(
    glm::vec3 _lightAmbient,
    glm::vec3 _lightDiffuse,
    glm::vec3 _lightSpecular
) {
    lightAmbient = _lightAmbient;
    lightDiffuse = _lightDiffuse;
    lightSpecular = _lightSpecular;
}

void LightDirectional::applyLightDirProperties() {
    glUniform3f(glGetUniformLocation(program, lightDirName.c_str()), lightDir.x, lightDir.y, lightDir.z);
}


void LightDirectional::applyLightProperties() {
    LightDirectional::applyLightColorProperties();
    lightDir = lightPos;
    LightDirectional::applyLightDirProperties();
}
