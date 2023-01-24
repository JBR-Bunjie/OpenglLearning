#include "Material.h"

Material::Material(GLuint _program) {
	Program = _program;
};

// set normal rgb value
void Material::setAmbientValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z) {
	ambient = glm::vec3(_x, _y, _z);
	ambientTarget = target;
}

void Material::setDiffuseValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z) {
	diffuseIsTexture = false;
	diffuse_xyz = glm::vec3(_x, _y, _z);
	diffuseTarget = target;
}

void Material::setSpecularValue(const GLchar* target, GLfloat _x, GLfloat _y, GLfloat _z) {
	specularIsTexture = false;
	specular_xyz = glm::vec3(_x, _y, _z);
	specularTarget = target;
}

void Material::setShininessValue(const GLchar* target, GLfloat _x) {
	shininess = _x;
	shininessTarget = target;
}

// set certain texture
void Material::setDiffuseValue(const GLchar* target, GLuint _texture) {
	diffuseIsTexture = true;
	diffuse_tex = _texture;
	diffuseTarget = target;
}

void Material::setSpecularValue(const GLchar* target, GLuint _texture) {
	specularIsTexture = true;
	specular_tex = _texture;
	specularTarget = target;
}

void Material::setEmissionValue(const GLchar* target, GLuint _texture) {
	emissionTexExist = true;
	emission_tex = _texture;
	emissionTarget = target;
}


// 
void Material::applyMaterial() {
	glUniform3f(glGetUniformLocation(Program, ambientTarget), ambient.x, ambient.y, ambient.z);

	if (!diffuseIsTexture) {
		glUniform3f(glGetUniformLocation(Program, diffuseTarget), diffuse_xyz.x, diffuse_xyz.y, diffuse_xyz.z);
	} else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_tex);
		glUniform1i(glGetUniformLocation(Program, diffuseTarget), 0);
	}

	if (!specularIsTexture) {
		glUniform3f(glGetUniformLocation(Program, specularTarget), specular_xyz.x, specular_xyz.y, specular_xyz.z);
	} else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_tex);
		glUniform1i(glGetUniformLocation(Program, specularTarget), 1);
	}

	if (emissionTexExist) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emission_tex);
		glUniform1i(glGetUniformLocation(Program, emissionTarget), 2);
	}

	glUniform1f(glGetUniformLocation(Program, shininessTarget), shininess);
}
