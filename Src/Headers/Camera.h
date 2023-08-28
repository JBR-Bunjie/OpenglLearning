#pragma once
#ifndef Camera
// #define Camera
//
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera();
    float pitch = 0.0f;     // 俯仰，一般在右手系x轴
    float yaw = -90.0f;     // 偏航，一般在右手系y轴
    // float roll;             // 翻滚，一般在右手系z轴，一般不采用此配置项
    float maxPitch = 89.0f;
    float minPitch = -89.0f;
    bool firstMouse = true;

    float lastX;
    float lastY;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;
    
    float fov = 45.0f;
    // fov is a component of projection matrix, cot(fov / 2) / aspect = mat11
    // take unity as an example:
    //      float t = unity_CameraProjection._m11;
    //      const float Rad2Deg = 180 / UNITY_PI;
    //      float fov = atan( 1.0f / t ) * 2.0 * Rad2Deg;
    // we will recover this content later to contrust a projection matrix by ourselves
    float near = 0.1f;
    float far = 100.0f;

    float frameDeltaTime;
    
    float mouseSensitivity;

    float movingSpeed;
    float targetMovingSpeed;
    
    glm::mat4 getViewMatrix();
    void RefreshMovingSpeed();
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    float GetZoom();
private:
    float zoom;

    void UpdateCameraVectors();
};

#endif