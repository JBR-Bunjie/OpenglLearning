#pragma once

#include "Shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Material
{
public:
	// program ID
	GLuint Program;

	glm::vec3 ambient;
	const GLchar* ambientTarget;

	glm::vec3 diffuse_xyz;
	GLuint diffuse_tex;
	const GLchar* diffuseTarget;

	glm::vec3 specular_xyz;
	GLuint specular_tex;
	const GLchar* specularTarget;

	GLuint emission_tex;
	const GLchar* emissionTarget;

	GLfloat shininess;
	const GLchar* shininessTarget;



	Material(GLuint _program);
	//~Material();

	void setAmbientValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z);
	void setDiffuseValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z);
	void setSpecularValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z);
	void setDiffuseValue(const GLchar* target, GLuint _texture);
	void setSpecularValue(const GLchar* target, GLuint _texture);
	void setEmissionValue(const GLchar* target, GLuint _texture);
	void setShininessValue(const GLchar* target, GLfloat _x);
	void applyMaterial();
private:
	bool diffuseIsTexture = false;
	bool specularIsTexture = false;
	bool emissionTexExist = false;
};

