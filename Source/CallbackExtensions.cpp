#include "Header/CallbackExtensions.h"

CallbackExtensions::CallbackExtensions() {
    return;
}

void CallbackExtensions::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void CallbackExtensions::ProcessInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}