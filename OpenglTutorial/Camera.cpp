#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldup) {
	position = _position;
	worldUp = _worldup;
	forward = glm::normalize(_target - position);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

Camera::Camera(glm::vec3 _position, float _pitch, float _yaw, glm::vec3 _worldup) {
	position = _position;
	worldUp = _worldup;
	pitch = _pitch;
	yaw = _yaw;
	forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

glm::mat4 Camera::GetViewMatrix() {
	//glm::vec3 temp = position + forward;
	//printf("%f, %f, %f\n", position.x, position.y, position.z);
	//printf("%f, %f, %f\n", forward.x, forward.y, forward.z);
	//printf("%f, %f, %f\n", temp.x, temp.y, temp.z);

	return glm::lookAt(position, position + forward, worldUp);
}

Camera::~Camera() {}

void Camera::processMouseMovement(float _deltaX, float _deltaY) {
	pitch -= _deltaY * mouseSensitivityY;
	yaw -= _deltaX * mouseSensitivityX;

	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	// yaw == 180
	//if (yaw > 89.0f)
	//	yaw = 89.0f;
	//else if (yaw < -89.0f)
	//	yaw = -89.0f;


	// printf("%f, %f\n", pitch, yaw);

	Camera::updateCameraVectors();
}

void Camera::updateCameraVectors() {
	forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::updateCameraPosition() {
	position += forward * speedZ * keySensitivityZ;
	position += right * speedX * keySensitivityX;
	position += up * speedY * keySensitivityY;

	yaw += yaw_move_speed * yawMoveSensitivity;
	Camera::updateCameraVectors();
}