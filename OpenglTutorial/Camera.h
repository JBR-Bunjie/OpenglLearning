#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldup);
	Camera(glm::vec3 _position, float _pitch, float _yaw, glm::vec3 _worldup);
	//Camera(glm::vec3 _position, float pitch, float yell, float roll, glm::vec3 worldUp);
	~Camera();

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	float pitch;
	float yaw;

	float yaw_move_speed = 0;

	float speedX = 0;
	float speedY = 0;
	float speedZ = 0;

	//glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 GetViewMatrix();
	void processMouseMovement(float _deltaX, float _deltaY);
	void updateCameraPosition();
private:
	void updateCameraVectors();

	float mouseSensitivityY = 0.02f;
	float mouseSensitivityZ = 0.02f;
	float mouseSensitivityX = 0.02f;

	float keySensitivityX = 0.02f;
	float keySensitivityY = 0.02f;
	float keySensitivityZ = 0.03f;
	
	float yawMoveSensitivity = 0.3f;
};

