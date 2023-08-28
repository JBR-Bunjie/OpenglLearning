#include "../Headers/Camera.h"

Camera::Camera() {
    return;
};

// Camera::Camera(float yaw, float pitch, float roll) {
//     this->yaw = yaw;
//     this->pitch = pitch;
//     this->roll = roll;
// };

void Camera::RefreshMovingSpeed() {
    movingSpeed = targetMovingSpeed * frameDeltaTime;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > maxPitch) {
        pitch = maxPitch;
    }
    if (pitch < -maxPitch) {
        pitch = -maxPitch;
    }

    // UpdateCameraVectors();
    
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
    
    // front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
    // front.y = sin(glm::radians(mainCamera.pitch));
    // front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
    // mainCamera.cameraFront = glm::normalize(front);
    // view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);
    //
    // proj = glm::perspective(glm::radians(mainCamera.fov), (float)viewportWidth / (float)viewportHeight, mainCamera.near, mainCamera.far);
}