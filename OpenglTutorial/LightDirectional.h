#pragma once

#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>

#include "BasicLight.h"

class LightDirectional : public BasicLight {
public:
    LightDirectional(glm::vec3 _lightPos, glm::vec3 _lightAmbient, glm::vec3 _lightDiffuse, glm::vec3 _lightSpecular);
    ~LightDirectional();

    void updateColor(glm::vec3 _lightAmbient, glm::vec3 _lightDiffuse, glm::vec3 _lightSpecular);

    void applyLightProperties();
private:
    glm::vec3 lightDir;
    std::string lightDirName = "lightDir";
    void applyLightDirProperties();
};
