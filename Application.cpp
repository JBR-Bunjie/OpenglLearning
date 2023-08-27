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
    mainCamera.cameraPos = glm::vec3(0.0f, 0.0f, 155.0f);
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

    // unsigned int InstancePlanetVAO, InstancePlanetVBO;
    // glGenVertexArrays(1, &InstancePlanetVAO);
    // glGenBuffers(1, &InstancePlanetVBO);
    // glBindVertexArray(InstancePlanetVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, InstancePlanetVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glBindVertexArray(0);
    // 
    // glm::vec2 translations[100];
    // int index = 0;
    // float offset = 0.1f;
    // for(int y = -10; y < 10; y += 2) {
    //     for(int x = -10; x < 10; x += 2) {
    //         glm::vec2 translation;
    //         translation.x = (float)x / 10.0f + offset;
    //         translation.y = (float)y / 10.0f + offset;
    //         translations[index++] = translation;
    //     }
    // }
    // unsigned int instanceOffsetVBO;
    // glGenBuffers(1, &instanceOffsetVBO);
    // glBindVertexArray(InstancePlanetVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // glVertexAttribDivisor(2, 1);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // // instancePlanetShader.use();
    // // for(unsigned int i = 0; i < 100; i++) {
    // //     std::stringstream ss;
    // //     string index;
    // //     ss << i;
    // //     index = ss.str();
    // //     instancePlanetShader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
    // // }


    /* Model */
    Model PlanetModel(PlanetModelSource);
    Model RockModel(RockModelSource);

    
    unsigned int ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /* Shader */
    Shader skyboxShader(skyboxVertexShaderSource, skyboxFragmentShaderSource);
    Shader instancePlanetShader(InstancePlanetVertexShaderSource, InstancePlanetFragmentShaderSource);
    Shader instanceAsteroidShader(InstanceAsteroidVertexShaderSource, InstanceAsteroidFragmentShaderSource);

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

        

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    unsigned int uniformBlockIndexSkybox = glGetUniformBlockIndex(skyboxShader.ID, "Matrices");
    glUniformBlockBinding(skyboxShader.ID, uniformBlockIndexSkybox, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    unsigned int amount = 100000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < RockModel.meshes.size(); i++) {
        unsigned int VAO = RockModel.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

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

        glm::vec3 front;
        front.x = cos(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        front.y = sin(glm::radians(mainCamera.pitch));
        front.z = sin(glm::radians(mainCamera.yaw)) * cos(glm::radians(mainCamera.pitch));
        mainCamera.cameraFront = glm::normalize(front);
        view = glm::lookAt(mainCamera.cameraPos, mainCamera.cameraPos + mainCamera.cameraFront, mainCamera.cameraUp);

        proj = glm::perspective(glm::radians(mainCamera.fov), (float)initialViewportWidth / (float)initialViewportHeight, 0.1f, 300.0f);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        // render
        // ------
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))
        
        // draw planet
        // instancePlanetShader.use();
        // glBindVertexArray(InstancePlanetVAO);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);
        // glBindVertexArray(0);
        // 绘制行星
        instancePlanetShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        instancePlanetShader.setMat4("model", model);
        instancePlanetShader.setMat4("view", view);
        instancePlanetShader.setMat4("proj", proj);
        PlanetModel.Draw(instancePlanetShader);
        // 绘制小行星
        instanceAsteroidShader.use();
        instanceAsteroidShader.setMat4("view", view);
        instanceAsteroidShader.setMat4("proj", proj);
        // PlanetModel.Draw(instancePlanetShader);
        for(unsigned int i = 0; i < RockModel.meshes.size(); i++) {
            glBindVertexArray(RockModel.meshes[i].VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES, RockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
            );
        }

        // Loop Final. 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
#pragma endregion
    skyboxShader.close();
    glfwTerminate();
    return 0;
}
