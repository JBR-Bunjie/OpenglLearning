#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Src/Headers/stb_image.h"

#include "Src/Sources/Shader.cpp"

#include "Src/Headers/GlobalSettings.h"
#include "Src/Headers/Tools.h"

#include "Src/Sources/CallbackExtensions.cpp"
#include "Src/Sources/Camera.cpp"
#include "Src/Sources/Light.cpp"

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
    
    Light additionalPointLights[4];
    additionalPointLights[0].lightType = LightType::Point;
    additionalPointLights[0].position = pointLightPositions[0];
    additionalPointLights[1].lightType = LightType::Point;
    additionalPointLights[1].position = pointLightPositions[1];
    additionalPointLights[2].lightType = LightType::Point;
    additionalPointLights[2].position = pointLightPositions[2];
    additionalPointLights[3].lightType = LightType::Point;
    additionalPointLights[3].position = pointLightPositions[3];

    Light additionalSpotLight;
    additionalSpotLight.lightType = LightType::Spot;
    additionalSpotLight.position = mainCamera.cameraPos;
    additionalSpotLight.direction = mainCamera.cameraFront;
    additionalSpotLight.cutOff = glm::cos(glm::radians(12.5f));
    additionalSpotLight.outerCutOff = glm::cos(glm::radians(15.0f));
#pragma endregion

#pragma region Buffer Init
    unsigned int BoxVAO[1];
    glGenVertexArrays(1, BoxVAO);
    unsigned int LightVAO[1];
    glGenVertexArrays(1, LightVAO);
    unsigned int VBO[1];
    glGenBuffers(1, VBO);
    unsigned int EBO[1];
    glGenBuffers(1, EBO);

    // 0. Set up VBO, which is the most important thing
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 1. Box
    // 1.1 Set up VAO for current Context
    glBindVertexArray(BoxVAO[0]);
    // // 1.2 EBO part
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 1.3 指针开始选取，一个VAO下一般至少有16个指针位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // 1.4 解绑VAO
    glBindVertexArray(0);
    
    // 2. Light, just like Box part
    glBindVertexArray(LightVAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 3. Set up textures
    unsigned int texture[2];
    texture[0] = RegisterTexture(container2TexSource, 0, TextureFormatPattern::RGBA);
    texture[1] = RegisterTexture(container2SpecTexSource, 1, TextureFormatPattern::RGBA);

    // Final. Setup Shader Program
    Shader BoxShader(BoxVertexShaderSource, BoxFragmentShaderSource);
    Shader LightShader(LightVertexShaderSource, LightFragmentShaderSource);
#pragma endregion

#pragma region Init Transform Matrix Calculation
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
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        // Refresh View and Proj Matrix which suit for nearly every object
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


        // 0. Box
        // Box - inject uniform data
        GLCall(BoxShader.use())
        // if (useGlobalAmbient) {
        //     BoxShader.setVec3("ambientColor", globalAmbientColor);
        // } else { ... }
        GLCall(BoxShader.setInt("material.diffuse", 0))
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        GLCall(BoxShader.setInt("material.specular", 1))
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        BoxShader.setFloat("material.shininess", 64.0f);
        // BoxShader.setVec3("material.ambient",  glm::vec3(1.0f, 0.5f, 0.31f));
        // BoxShader.setVec3("material.diffuse",  glm::vec3(1.0f, 0.5f, 0.31f));
        // BoxShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        BoxShader.setFloat("ambientStrength", globalAmbientStrength);
        BoxShader.setFloat("specularStrength", specularStrength);
        BoxShader.setFloat("shininess", shininess);
        BoxShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        BoxShader.setVec3(  "directionalLight.direction", mainLight.direction);
        BoxShader.setVec3(  "directionalLight.ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        BoxShader.setVec3(  "directionalLight.diffuse",  glm::vec3(0.4f, 0.4f, 0.4f));
        BoxShader.setVec3(  "directionalLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        BoxShader.setVec3(  "pointLight[0].position", additionalPointLights[0].position);
        BoxShader.setFloat( "pointLight[0].constant", additionalPointLights[0].constant);
        BoxShader.setFloat( "pointLight[0].linear", additionalPointLights[0].linear);
        BoxShader.setFloat( "pointLight[0].quadratic", additionalPointLights[0].quadratic);
        BoxShader.setVec3(  "pointLight[0].ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        BoxShader.setVec3(  "pointLight[0].diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
        BoxShader.setVec3(  "pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        BoxShader.setVec3(  "pointLight[1].position", additionalPointLights[1].position);
        BoxShader.setFloat( "pointLight[1].linear", additionalPointLights[1].linear);
        BoxShader.setFloat( "pointLight[1].quadratic", additionalPointLights[1].quadratic);
        BoxShader.setVec3(  "pointLight[2].position", additionalPointLights[2].position);
        BoxShader.setVec3(  "pointLight[1].ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        BoxShader.setVec3(  "pointLight[1].diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
        BoxShader.setVec3(  "pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        BoxShader.setFloat( "pointLight[1].constant", additionalPointLights[1].constant);
        BoxShader.setFloat( "pointLight[2].constant", additionalPointLights[2].constant);
        BoxShader.setFloat( "pointLight[2].linear", additionalPointLights[2].linear);
        BoxShader.setFloat("pointLight[2].quadratic", additionalPointLights[2].quadratic);
        BoxShader.setVec3(  "pointLight[2].ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        BoxShader.setVec3(  "pointLight[2].diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
        BoxShader.setVec3(  "pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        BoxShader.setVec3(  "pointLight[3].position", additionalPointLights[3].position);
        BoxShader.setFloat( "pointLight[3].constant", additionalPointLights[3].constant);
        BoxShader.setFloat( "pointLight[3].linear", additionalPointLights[3].linear);
        BoxShader.setFloat( "pointLight[3].quadratic", additionalPointLights[3].quadratic);
        BoxShader.setVec3(  "pointLight[3].ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        BoxShader.setVec3(  "pointLight[3].diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
        BoxShader.setVec3(  "pointLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        additionalSpotLight.position = mainCamera.cameraPos;
        additionalSpotLight.direction = mainCamera.cameraFront;
        BoxShader.setVec3(  "spotLight.position", additionalSpotLight.position);
        BoxShader.setVec3(  "spotLight.direction", additionalSpotLight.direction);
        BoxShader.setFloat( "spotLight.cutOff", additionalSpotLight.cutOff);
        BoxShader.setFloat( "spotLight.outerCutOff", additionalSpotLight.outerCutOff);
        BoxShader.setVec3(  "spotLight.ambient",  glm::vec3(0.0f, 0.0f, 0.0f));
        BoxShader.setVec3(  "spotLight.diffuse",  glm::vec3(1.0f, 1.0f, 1.0f));
        BoxShader.setVec3(  "spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        BoxShader.setFloat( "spotLight.constant", additionalSpotLight.constant);
        BoxShader.setFloat( "spotLight.linear", additionalSpotLight.linear);
        BoxShader.setFloat( "spotLight.quadratic", additionalSpotLight.quadratic);
        
        // Box - Bind VAO 
        GLCall(glBindVertexArray(BoxVAO[0]))
        // Box - Draw&Call Loop
        for (unsigned int i = 0; i < 10; i++) {
            // Box - Refresh Model Matrix
            float angle = 20.0f * i; 
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            BoxShader.setMat4("model", model);
            BoxShader.setMat4("view", view);
            BoxShader.setMat4("proj", proj);
            BoxShader.setVec3("lightPos", mainLight.position);
            BoxShader.setVec3("camPos", mainCamera.cameraPos);

            // Box - draw&call
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }


        // 1. Light
        // Light - Bind Shader and VAO
        GLCall(LightShader.use())
        GLCall(glBindVertexArray(LightVAO[0]))

        // Light - Draw&Call Loop
        for (int i = 0; i < 4; i++) {
            // Light - Refresh Matrix
            model = glm::mat4(1.0f);
            model = glm::translate(model, additionalPointLights[i].position);
            model = glm::scale(model, glm::vec3(0.2f));
        
            // Light - inject uniform data
            LightShader.setMat4("model", model);
            LightShader.setMat4("view", view);
            LightShader.setMat4("proj", proj);

            // Light - draw&call
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion

    BoxShader.close();
    LightShader.close();
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, BoxVAO);
    glfwTerminate();
    return 0;
}
