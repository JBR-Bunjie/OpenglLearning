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

using std::string;

#pragma region Model Data
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
#pragma endregion

#pragma region Function Declare
GLuint load_image_to_gpu(const char* filename, GLint internalFormat, GLenum format, GLint textureSlotOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
#pragma endregion

#pragma region Camera Declare
//Camera camera = Camera::Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Camera camera = Camera::Camera(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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
    Shader localShader("vertexShader.glsl", "fragmentShader.glsl");
#pragma endregion

    #pragma region Init And Loads Model to VAO and VBO
        GLuint VBO[1];
        glGenBuffers(1, VBO);

        GLuint VAO[1];
        glGenVertexArrays(1, VAO);


        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        //glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    #pragma endregion

    #pragma region Init and Load Textures
        GLuint texture[2];
        glGenTextures(2, texture);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture[0] = load_image_to_gpu("container.jpg", GL_RGB, GL_RGB, 0);

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture[1] = load_image_to_gpu("awesomeface.png", GL_RGBA, GL_RGBA, 1);
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set Materials -> Shader Program
        localShader.Use();

        // Calculate View And Projection Matrices Here
        view = camera.GetViewMatrix();


        // Set Materials -> Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUniform1i(glGetUniformLocation(localShader.Program, "ourTexture0"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(localShader.Program, "ourTexture1"), 1);
        glUniform1f(glGetUniformLocation(localShader.Program, "texture_mix_control"), texture_mix_control);

        // Use MVP
        glUniformMatrix4fv(glGetUniformLocation(localShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(localShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(localShader.Program, "project"), 1, GL_FALSE, glm::value_ptr(project));


        for (GLuint i = 0; i < 10; i++) {
            // Set trans Matrix
            GLfloat angle = 20.0f * i;
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            trans = glm::rotate(trans, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(localShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

            // Draw & Call
            glBindVertexArray(VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

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