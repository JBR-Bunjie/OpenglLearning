#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// matrix computation
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#include <iostream>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Material.h"

using std::string;

#pragma region Model Data
GLfloat vertices[] = {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion

#pragma region Global Settings
const int windowWidth = 1600;
const int windowHeight = 1200;

float texture_mix_control = 0.2f;
#pragma endregion

#pragma region Global Variables
float lastX, lastY;
bool firstMouse = true;

glm::vec3 lightPos(0.5f, 0.5f, 1.0f);
#pragma endregion

#pragma region Function Declare
GLuint load_image_to_gpu(const char* filename, GLint internalFormat, GLenum format, GLint textureSlotOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
#pragma endregion

#pragma region Camera Declare
//Camera camera = Camera::Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Camera camera = Camera::Camera(glm::vec3(0.3f, 0.5f, 3.0f), -15.0f, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
#pragma endregion

int main(void) {
    #pragma region Open A Window And Init
    // Open GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
#pragma endregion

    #pragma region Set Render Conditions
        //glCullFace(GL_FRONT);
        //glCullFace(GL_BACK);
        //glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    #pragma endregion

    #pragma region Init Shader Program
    Shader objectShader("objectVertexShader.glsl", "objectFragmentShader.glsl");
    Shader lightShader("lightVertexShader.glsl", "lightFragmentShader.glsl");
    #pragma endregion

    #pragma region Init Light Color
        glm::vec3 lightColor = glm::vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
#pragma endregion

    #pragma region Init And Loads Model to VAO and VBO
        GLuint VBO[1];
        glGenBuffers(1, VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint objectVAO, lightVAO;                                               
        glGenVertexArrays(1, &objectVAO);
        glGenVertexArrays(1, &lightVAO);

        glBindVertexArray(objectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    #pragma endregion

    #pragma region Init Material
        // Init Materials
        GLuint texture_diff = load_image_to_gpu("container2.png", GL_RGBA, GL_RGBA, 0);
        GLuint texture_spec = load_image_to_gpu("container2_specular.png", GL_RGBA, GL_RGBA, 1);
        GLuint texture_emis = load_image_to_gpu("matrix.jpg", GL_RGB, GL_RGB, 2);

        Material objectMaterial = Material::Material(objectShader.Program);
        objectMaterial.setAmbientValue("material.ambient", 1.0f, 0.5f, 0.31f);
        //objectMaterial.setDiffuseValue("material.diffuse", 1.0f, 0.5f, 0.31f);
        //objectMaterial.setSpecularValue("material.specular", 0.5f, 0.5f, 0.5f);
        objectMaterial.setDiffuseValue("material.diffuse", texture_diff);
        objectMaterial.setSpecularValue("material.specular", texture_spec);
        objectMaterial.setEmissionValue("material.emission", texture_emis);
        objectMaterial.setShininessValue("material.shininess", 32.0f);
    #pragma endregion

    #pragma region Declare MVP Matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 project = glm::mat4(1.0f);
        project = glm::perspective(45.0f, float(windowWidth / windowHeight), 0.1f, 100.0f);
    #pragma endregion

    glfwSetKeyCallback(window, key_callback); // 注册我们定义的按键回调函数
    glfwSetCursorPosCallback(window, mouse_callback);
    while (!glfwWindowShouldClose(window)) {
        // Clear Screen And Fill Basic Color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate View And Projection Matrices Here
        view = camera.GetViewMatrix();

        // Set Materials -> Shader Program -> OB
        objectShader.Use();

        // Use MVP
        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "project"), 1, GL_FALSE, glm::value_ptr(project));

        // Set trans Matrix
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 1.5f));
        trans = glm::scale(trans, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(objectShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        // Set Light Color
        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);
        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        GLint lightAmbientLoc = glGetUniformLocation(objectShader.Program, "light.ambient");
        GLint lightDiffuseLoc = glGetUniformLocation(objectShader.Program, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(objectShader.Program, "light.specular");
        glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        // Set LightPos
        glUniform3f(glGetUniformLocation(objectShader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        // Set Camera Pos
        GLint viewPosLoc = glGetUniformLocation(objectShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

        // apply material
        objectMaterial.applyMaterial();

        // Draw & Call
        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Set Materials -> Shader Program -> LS
        lightShader.Use();

        // Use MVP
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "project"), 1, GL_FALSE, glm::value_ptr(project));

        // Use trans
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, lightPos);
        trans = glm::scale(trans, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        // Set lightColor
        glUniform3f(glGetUniformLocation(lightShader.Program, "lightColor"), 1.0f, 1.0f, 1.0f); // 依旧把光源设置为白色

        // Draw & Call
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        // Clean Up, Prepare for Next Window Loop
        glfwSwapBuffers(window);
        glfwPollEvents();
        camera.updateCameraPosition();
    }

    // Exit Program
    glfwTerminate();
	return 0;
}

#pragma region Function Units
GLuint load_image_to_gpu(const char* filename, GLint internalFormat, GLenum format, GLint textureSlotOffset) {
    GLuint _texture;
    glGenTextures(1, &_texture);
    glActiveTexture(GL_TEXTURE0 + textureSlotOffset);
    glBindTexture(GL_TEXTURE_2D, _texture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "failed to load images." << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return _texture;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        texture_mix_control += 0.1f;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        texture_mix_control -= 0.1f;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        camera.speedZ = 1.0f;
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        camera.speedZ = -1.0f;
    else {
        camera.speedZ = 0.0f;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        camera.speedX = 1.0f;
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
        camera.speedX = -1.0f;
    else {
        camera.speedX = 0.0f;
    }

    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        camera.speedY = -1.0f;
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        camera.speedY = 1.0f;
    else {
        camera.speedY = 0.0f;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        camera.yaw_move_speed = -1.0f;
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        camera.yaw_move_speed = 1.0f;
    else {
        camera.yaw_move_speed = 0.0f;
    }
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float deltaX = xPos - lastX;
    float deltaY = yPos - lastY;

    lastX = xPos;
    lastY = yPos;
    //std::cout << deltaX << ", " << deltaY << std::endl;
    //std::cout << xPos << ", " << yPos << std::endl;
    camera.processMouseMovement(deltaX, deltaY);
}
#pragma endregion