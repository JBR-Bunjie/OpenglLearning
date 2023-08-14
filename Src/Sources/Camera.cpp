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