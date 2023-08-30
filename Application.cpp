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
#include <sstream>
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

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
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
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
#pragma endregion

#pragma region Data Defination
    /* Game Components */
    // Components Init
    Camera mainCamera;
    mainCamera.mouseSensitivity = 0.05f;
    mainCamera.targetMovingSpeed = 2.5f;
    mainCamera.fov = 45.0f;
    mainCamera.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    mainCamera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mainCamera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // 
    mainCamera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // 
    // // Light
    // Light mainLight;
    // mainLight.lightType = LightType::Directional;
    // Light additionalPointLight;
    // additionalPointLight.lightType = LightType::Point;

    /* Matrices Setup */
    // 0. model
    glm::mat4 model;
    // 1. view
    glm::mat4 view;
    // 2. proj
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(mainCamera.fov), (float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);

    
    /* Model */


    /* Buffer Data */
    // Setup FBO which generate shadowMap
    unsigned int ShadowMapFBO;
    glGenFramebuffers(1, &ShadowMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // 可以看到这里的贴图取样设置中，环绕方式设置成了GL_REPEAT
    // // 这意味着，当我们超出这个区域时，我们会重复之前的深度值——这很有可能造成不应该存在的阴影
    // // 在我们的案例中表现为：光照有一个区域，超出该区域就成为了阴影，而这个区域实际上代表着深度贴图的大小，因为这个贴图投影到了地板上。
    // // 我们宁可让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中。
    // // 我们可以储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // 当然，这样子是不足以解决所有的问题的。除了阴影贴图本身的大小等因素外，导致区域外呈现阴影还有很多原因
    // 比如：渲染物体的坐标超出了光的正交视锥的远平面等（这里采用了Learn上面的做法，直接对超限坐标设shadow = 0

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    
    /* Shader */
    Shader shadowMapGenerateShader(ShadowMap_Directional_Generate_Vertex, ShadowMap_Directional_Generate_Fragment);
    Shader shadowMapApplyShader(ShadowMap_Directional_Apply_Vertex, ShadowMap_Directional_Apply_Fragment);

    /* Texture */
    unsigned int woodTexture = RegisterTexture(woodTexSource);
#pragma endregion

#pragma region State Defination
    /* Opengl Settings */
    // Drawing Type:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Face Culling:
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    // Depth:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Stencil:
    glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // It should be specified for every object. So I just comment this line out.
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // to be honest, this one should be specified just like the above one. But in this case, it suit for every case.

    /* Register Callback */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, CallbackExtensions::FramebufferSizeCallback);
    // glfwSetCursorEnterCallback(window, CallbackExtensions::CursorEnterCallback);
    glfwSetCursorPosCallback(window, CallbackExtensions::MouseCallback);
    glfwSetScrollCallback(window, CallbackExtensions::ScrollCallback);
#pragma endregion

#pragma region Render Loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        mainCamera.frameDeltaTime = deltaTime;

        // input
        // -----
        CallbackExtensions::ProcessInput(window, mainCamera);
        
        // Pass 1: Shadow Pass
        // -------------------
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        float tempNearPlane = 1.0f, tempFarPlane = 7.5f;
        glm::vec3 lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
        proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, tempNearPlane, tempFarPlane);
        view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shadowMapGenerateShader.use();
        glm::mat4 lightSpaceMatrix = proj * view;
        shadowMapGenerateShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        renderScene(shadowMapGenerateShader);
        

        // Pass 2: Normal Pass
        // -------------------
        glViewport(0, 0, viewportWidth, viewportHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.5, 0.5, 0.5, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        shadowMapApplyShader.use();
        proj = glm::perspective(glm::radians(mainCamera.fov), (float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);
        view = mainCamera.getViewMatrix();
        shadowMapApplyShader.setMat4("proj", proj);
        shadowMapApplyShader.setMat4("view", view);
        // set light uniforms
        shadowMapApplyShader.setVec3("viewPos", mainCamera.cameraPos);
        shadowMapApplyShader.setVec3("lightPos", lightPos);
        shadowMapApplyShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        shadowMapApplyShader.setInt("diffuseTexture", 0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        shadowMapApplyShader.setInt("shadowMap", 1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shadowMapApplyShader);

        // Pass Debug: Show ShadowMap
        // shadowMapApplyShader.use();
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // shadowMapApplyShader.setFloat("near_plane", tempNearPlane);
        // shadowMapApplyShader.setFloat("far_plane", tempFarPlane);
        // shadowMapApplyShader.setInt("depthMap", 0);
        // renderQuad();
        

        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion
    shadowMapGenerateShader.close();
    shadowMapApplyShader.close();
    glfwTerminate();
    return 0;
}
