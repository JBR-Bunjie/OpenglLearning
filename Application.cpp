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
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initialViewportWidth, initialViewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, initialViewportWidth, initialViewportHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    /* Textures */
    unsigned int cubeTexture = RegisterTexture(containerTexSource);
    unsigned int floorTexture = RegisterTexture(floorTexSource);

    /* Shader */
    Shader shader(stencilTestVertexShaderSource, stencilTestFragmentShaderSource);
    Shader shaderSingleColor(stencilTestVertexShaderSource, stencilSingleColorFragmentShaderSource);
    Shader screenShader(offscreenVertexShaderSource, offscreenFragmentShaderSource);
    
    shader.use();
    shader.setInt("texture1", 0);
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
#pragma endregion

#pragma region State Defination
    /* Opengl Settings */
    // Drawing Type:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Face Culling:
    // glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // Depth:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Stencil:
    glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // It should be specified for every object. So I just comment this line out.
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // to be honest, this one should be specified just like the above one. But in this case, it suit for every case.

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
        shaderSingleColor.use();
        model = glm::mat4(1.0f);
        glm::vec3 front;
        front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        front.y = sin(glm::radians(mainCamera.pitch));
        front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        mainCamera.cameraFront = glm::normalize(front);
        view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);

        proj = glm::perspective(glm::radians(mainCamera.fov),
                                (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);
        
        shaderSingleColor.setMat4("view", view);
        shaderSingleColor.setMat4("proj", proj);
        
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("proj", proj);

        // 第一处理阶段(Pass)
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // render
        // ------
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))
        
        // Plane的渲染，模板无关，先渲染，并且使用遮罩防止它修改模板缓冲
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0x00);
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 此时模板缓冲应该仍然全为0，开始修改Func与Mask，现在，在Cube对应位置的模板缓冲应该为1
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 我们将Func修改为(GL_NOTEQUAL, 1, 0xFF)，并且使用的Mask为0x00防止写入(应该没必要,So I comment it out)
        // 只有模板缓冲中值不为1的部分才能通过，并且不会影响已有的模板缓冲，不然会被REPLACE(根据没有修改过的Op)
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST); //这里关闭了深度缓冲是另一个有意思的点，这样做是因为我们不想让立方体的边框被地板的边框遮挡
        shaderSingleColor.use();
        float scale = 1.1f;
        // cubes
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // 第二处理阶段
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT))
        
        screenShader.use();  
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);  

        // Re-Init
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        
        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    shader.close();
    shaderSingleColor.close();
    glfwTerminate();
    return 0;
}
