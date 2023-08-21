#include "../Headers/CallbackExtensions.h"
#include "../Headers/CallbackExtensions.h"


// CallbackExtensions::CallbackExtensions() {
//     return;
// }

void CallbackExtensions::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void CallbackExtensions::MouseCallback(GLFWwindow* window, double xpos_input, double ypos_input) {
    // std::cout << "Mouse Triggered" << std::endl;
    mouseXpos = xpos_input;
    mouseYpos = ypos_input;
}

void CallbackExtensions::ScrollCallback(GLFWwindow* window, double xoffset_input, double yoffset_input) {
    // std::cout << "Scroll Triggered" << yoffset_input << std::endl;
    scrollXoffset = xoffset_input;
    scrollYoffset = yoffset_input;
}

void CallbackExtensions::ProcessInput(GLFWwindow* window, Camera &camera) {
    /* Keyboard Input Detection */
    // 0. Menu Key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 1. Moving Key
    camera.RefreshMovingSpeed();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.cameraPos += camera.movingSpeed * camera.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.cameraPos -= camera.movingSpeed * camera.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * camera.movingSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * camera.movingSpeed;

    // 2. Mouse Input Processing
    if (camera.firstMouse) {
        camera.lastX = mouseXpos;
        camera.lastY = mouseYpos;
        camera.firstMouse = false;
    }
    
    float xoffset = mouseXpos - camera.lastX;
    float yoffset = camera.lastY - mouseYpos;
    camera.lastX = mouseXpos;
    camera.lastY = mouseYpos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    // 3. Scroll Input Processing
    if(camera.fov >= 1.0f && camera.fov <= 45.0f)
        camera.fov -= scrollYoffset;
    if(camera.fov <= 1.0f)
        camera.fov = 1.0f;
    if(camera.fov >= 45.0f)
        camera.fov = 45.0f;
    // std::cout << "fov changed: " << camera.fov << std::endl;
    // cout << "camera pos: " << camera.cameraPos.x << ", " << camera.cameraPos.y << ", " << camera.cameraPos.z << endl;
}