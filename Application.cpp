#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include "Src/Headers/stb_image.h"

#include "Src/Headers/ConstValues.h"
//#include "Src/Headers/CallbackExtensions.h"
#include "Src/Sources/CallbackExtensions.cpp"
#include "Src/Sources/Camera.cpp"
#include "Src/Sources/Shader.cpp"

#include "Src/Headers/Tools.h"

// float targetFrameRate = 60.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

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

#pragma region PreDefine
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
#pragma endregion

#pragma region Init Buffer Data
    unsigned int VAO[1];
    glGenVertexArrays(1, VAO);
    unsigned int VBO[1];
    glGenBuffers(1, VBO);
    unsigned int EBO[1];
    glGenBuffers(1, EBO);

    // 0. 指明顶点属性指针
    glBindVertexArray(VAO[0]);

    // 1. 设置VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // // 2. 设置EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 指针开始选取，一个VAO下一般至少有16个指针位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 4. Set up textures
    int width, height, nrChannels;
    unsigned char* data;
    unsigned int texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load(containerTexSource, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data))
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load(faceTexSource, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Final. Setup Shader Program
    Shader ShaderProgram(vertexShaderSource, fragmentShaderSource);
#pragma endregion

#pragma region Init Transform Matrix Calculation
    // 0. model matrix part
    glm::mat4 model;
    // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0f));

    // 1. view matrix part
    mainCamera.cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    mainCamera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mainCamera.cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 view;
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // 2. projection matrix part
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);

    // other
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
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        // -1. Time Check
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        mainCamera.frameDeltaTime = deltaTime;

        // 0. Box
        // Box - Set Texture
        GLCall(ShaderProgram.use())
        GLCall(ShaderProgram.setInt("baseTex0", 0))
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        GLCall(ShaderProgram.setInt("baseTex1", 1))
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        // Box - Draw Call
        GLCall(glBindVertexArray(VAO[0]))
        for (unsigned int i = 0; i < 10; i++) {
            // inject mvp matrix
            float angle = 20.0f * i; 
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            // float radius = 10.0f;
            // float camX = sin(glfwGetTime()) * radius;
            // float camZ = cos(glfwGetTime()) * radius;
            glm::vec3 front;
            front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
            front.y = sin(glm::radians(mainCamera.pitch));
            front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
            mainCamera.cameraFront = glm::normalize(front);
            view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);
            
            proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 100.0f);

            
            unsigned int modelLoc = glGetUniformLocation(ShaderProgram.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            unsigned int viewLoc = glGetUniformLocation(ShaderProgram.ID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            unsigned int projLoc = glGetUniformLocation(ShaderProgram.ID, "proj");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

            // draw&call
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion

    ShaderProgram.close();
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, VAO);
    glfwTerminate();
    return 0;
}
