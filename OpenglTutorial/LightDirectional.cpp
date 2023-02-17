#include "LightDirectional.h"
#include "BasicLight.h"

LightDirectional::LightDirectional(glm::vec3 _lightPos, glm::vec3 _lightAmbient, glm::vec3 _lightDiffuse, glm::vec3 _lightSpecular)
	:BasicLight::BasicLight(_lightPos, _lightAmbient, _lightDiffuse, _lightSpecular) {
	LightDirectional::updateDirection();
}

LightDirectional::~LightDirectional() {

}

void LightDirectional::updateDirection() {	

}