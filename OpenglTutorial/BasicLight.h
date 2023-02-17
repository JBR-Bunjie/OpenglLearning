#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.inl>
#include <gtx/rotate_vector.hpp>

#include <string>


class BasicLight
{
public:
	// Position
	// we use lightPos - origin, then the result is just the lightDir that we want.
	glm::vec3 lightPos;

	// Color
	// we still need the three value to set the light
	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;

	GLuint program;
	
	std::string lightName;

	BasicLight(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	~BasicLight();

	void updateDirection();

	void setProgram(GLuint _program, const GLchar* _lightName);

	void applyLightProperties();
};

