#pragma once
#ifndef CallbackExtensions

class CallbackExtensions {
public:
CallbackExtensions();
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void ProcessInput(GLFWwindow *window);
    
private:
    
};


#endif