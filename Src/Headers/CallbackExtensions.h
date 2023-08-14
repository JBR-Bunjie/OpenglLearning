#pragma once
#ifndef CallbackExtensions
//#define CallbackExtensions
#include "Camera.h"
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>

double mouseXpos;
double mouseYpos;
double scrollXoffset;
double scrollYoffset;
static class CallbackExtensions {
public:
    // CallbackExtensions();

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos_input, double ypos_input);
    static void ScrollCallback(GLFWwindow* window, double xoffset_input, double yoffset_input);
    static void ProcessInput(GLFWwindow* window, Camera &camera);
private:
    
};


#endif