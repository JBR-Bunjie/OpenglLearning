#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Src/Headers/stb_image.h"

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::cout;
using std::endl;

#include "Src/Sources/Shader.cpp"
#include "Src/Headers/GlobalSettings.h"
#include "Src/Headers/Tools.h"
#include "Src/Sources/Camera.cpp"
#include "Src/Sources/Light.cpp"
#include "Src/Sources/Mesh.cpp"
#include "Src/Sources/Model.cpp"
// #include "Src/Headers/Mesh.h"
// #include "Src/Headers/Model.h"
#include "Src/Sources/CallbackExtensions.cpp"

int main() {
#pragma region PrepareWindow
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    glfwGetFramebufferSize(window, &initialViewportWidth, &initialViewportHeight);
    glViewport(0, 0, initialViewportWidth, initialViewportHeight);
#pragma endregion

#pragma region State And Variables Defination
    // Opengl Settings
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glCullFace(GL_FRONT);
    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Components Init
    Camera mainCamera;
    mainCamera.mouseSensitivity = 0.05f;
    mainCamera.targetMovingSpeed = 2.5f;
    mainCamera.fov = 45.0f;
    mainCamera.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    mainCamera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mainCamera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // 
    mainCamera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // 

    Light mainLight;
    mainLight.lightType = LightType::Directional;
    mainLight.position = glm::vec3(1.2f, 1.0f, 2.0f);
    mainLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    // mainLight.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    // mainLight.lightStrength = 1.0f;
    
    // Light additionalPointLights[4];
    // additionalPointLights[0].lightType = LightType::Point;
    // additionalPointLights[0].position = pointLightPositions[0];
    // additionalPointLights[1].lightType = LightType::Point;
    // additionalPointLights[1].position = pointLightPositions[1];
    // additionalPointLights[2].lightType = LightType::Point;
    // additionalPointLights[2].position = pointLightPositions[2];
    // additionalPointLights[3].lightType = LightType::Point;
    // additionalPointLights[3].position = pointLightPositions[3];
    //
    // Light additionalSpotLight;
    // additionalSpotLight.lightType = LightType::Spot;
    // additionalSpotLight.position = mainCamera.cameraPos;
    // additionalSpotLight.direction = mainCamera.cameraFront;
    // additionalSpotLight.cutOff = glm::cos(glm::radians(12.5f));
    // additionalSpotLight.outerCutOff = glm::cos(glm::radians(15.0f));

    
    Model nanosuit(NanosuitModelSource);
    // Model backpack(BackpackModelSource);
    
    // Shader LightShader(LightVertexShaderSource, LightFragmentShaderSource);
    Shader NanosuitShader(NanosuitVertexShaderSource, NanosuitFragmentShaderSource);

    // 0. model matrix part
    glm::mat4 model;

    // 1. view matrix part
    glm::mat4 view;

    // 2. projection matrix part
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);
#pragma endregion

#pragma region Register Callback Function
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, CallbackExtensions::FramebufferSizeCallback);
    // glfwSetCursorEnterCallback(window, CallbackExtensions::CursorEnterCallback);
    glfwSetCursorPosCallback(window, CallbackExtensions::MouseCallback);
    glfwSetScrollCallback(window, CallbackExtensions::ScrollCallback);
#pragma endregion

#pragma region Render Loop
    while (!glfwWindowShouldClose(window)) {
        // 输入
        CallbackExtensions::ProcessInput(window, mainCamera);

        
        // 渲染指令
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        // Refresh View and Proj Matrix which suit for nearly every object
        model = glm::mat4(1.0f);

        glm::vec3 front;
        front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        front.y = sin(glm::radians(mainCamera.pitch));
        front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        mainCamera.cameraFront = glm::normalize(front);
        view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);

        proj = glm::perspective(glm::radians(mainCamera.fov),
                                (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);

        // -1. Time Check
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        mainCamera.frameDeltaTime = deltaTime;
        

        GLCall(NanosuitShader.use())
        GLCall(NanosuitShader.setMat4("model", model))
        GLCall(NanosuitShader.setMat4("view", view))
        GLCall(NanosuitShader.setMat4("proj", proj))
        GLCall(nanosuit.Draw(NanosuitShader))
        // GLCall(backpack.Draw(NanosuitShader))
        
        
        // // 1. Light
        // // Light - Bind Shader and VAO
        // GLCall(LightShader.use())
        // GLCall(glBindVertexArray(LightVAO[0]))
        //
        // // Light - Draw&Call Loop
        // for (int i = 0; i < 4; i++) {
        //     // Light - Refresh Matrix
        //     model = glm::mat4(1.0f);
        //     model = glm::translate(model, additionalPointLights[i].position);
        //     model = glm::scale(model, glm::vec3(0.2f));
        //
        //     // Light - inject uniform data
        //     LightShader.setMat4("model", model);
        //     LightShader.setMat4("view", view);
        //     LightShader.setMat4("proj", proj);
        //
        //     // Light - draw&call
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion

    // glDeleteBuffers(1, VBO);
    // glDeleteVertexArrays(1, BoxVAO);
    // LightShader.close();
    NanosuitShader.close();
    glfwTerminate();
    return 0;
}
