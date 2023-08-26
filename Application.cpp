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

    /* Matrices Setup */
    // 0. model matrix part
    glm::mat4 model;

    // 1. view matrix part
    glm::mat4 view;

    // 2. projection matrix part
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);


    /* Buffer Data */
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVForSkybox), &cubeVForSkybox, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    // unsigned int geometryShaderExampleAO, geometryShaderExampleBO;
    // glGenVertexArrays(1, &geometryShaderExampleAO);
    // glGenBuffers(1, &geometryShaderExampleBO);
    // glBindVertexArray(geometryShaderExampleAO);
    // glBindBuffer(GL_ARRAY_BUFFER, geometryShaderExampleBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // glBindVertexArray(0);
    
    /* Shader */
    Shader shader(box_skyboxVertexShaderSource, box_skyboxFragmentShaderSource);
    Shader skyboxShader(skyboxVertexShaderSource, skyboxFragmentShaderSource);
    // Shader GeometryExampleShader(GeometryShaderVertexShaderSource, GeometryShaderFragmentShaderSource, GeometryShaderGeometryShaderSource);
    Shader shaderWithoutGeometryStage(NanosuitVertexShaderSource, NanosuitFragmentShaderSource);
    Shader shaderWithGeometryStage(GeometryVisualizedNormalVertexShaderSource, GeometryVisualizedNormalFragmentShaderSource, GeometryVisualizedNormalGeometryShaderSource);
    
    
    Model nanosuit(NanosuitModelSource);

    /* Texture */
    vector<std::string> faces {
        skyboxTexrightSource,
        skyboxTexleftSource,
        skyboxTextopSource,
        skyboxTexbottomSource,
        skyboxTexfrontSource,
        skyboxTexbackSource
    };

    unsigned int skyboxTexture = loadCubemap(faces);
    
    shader.use();
    shader.setInt("skybox", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    unsigned int uniformBlockIndexSkybox = glGetUniformBlockIndex(shader.ID, "Matrices");
    unsigned int uniformBlockIndexBox = glGetUniformBlockIndex(skyboxShader.ID, "Matrices");
    unsigned int uniformBlockIndexNanosuit = glGetUniformBlockIndex(shaderWithoutGeometryStage.ID, "Matrices");
    glUniformBlockBinding(shader.ID, uniformBlockIndexSkybox, 0);
    glUniformBlockBinding(skyboxShader.ID, uniformBlockIndexBox, 0);
    glUniformBlockBinding(shaderWithoutGeometryStage.ID, uniformBlockIndexNanosuit, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
    
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
        
        // Refresh View and Proj Matrix which suit for nearly every object

        model = glm::mat4(1.0f);
        glm::vec3 front;
        front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        front.y = sin(glm::radians(mainCamera.pitch));
        front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        mainCamera.cameraFront = glm::normalize(front);
        view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);

        proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        // render
        // ------
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))
        
        // // draw scene as normal
        // shader.use();
        // shader.setMat4("model", model);
        // shader.setMat4("view", view);
        // shader.setMat4("proj", proj);
        // shader.setVec3("cameraPos", mainCamera.cameraPos);
        // // cubes
        // glBindVertexArray(cubeVAO);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // GeometryExampleShader.use();
        // glBindVertexArray(geometryShaderExampleAO);
        // glDrawArrays(GL_POINTS, 0, 4);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
        model = glm::scale(model, glm::vec3(0.2f));
        shaderWithoutGeometryStage.use();
        shaderWithoutGeometryStage.setMat4("model", model);
        nanosuit.Draw(shaderWithoutGeometryStage);
        shaderWithGeometryStage.use();
        shaderWithGeometryStage.setMat4("model", model);
        shaderWithGeometryStage.setMat4("view", view);
        shaderWithGeometryStage.setMat4("proj", proj);
        nanosuit.Draw(shaderWithGeometryStage);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        // glDisable(GL_CULL_FACE);
        skyboxShader.use();
        // view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        // skyboxShader.setMat4("view", view);
        // skyboxShader.setMat4("proj", proj);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        
        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion
    skyboxShader.close();
    glfwTerminate();
    return 0;
}
