#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Header/ConstValues.h"
#include "Source/CallbackExtensions.cpp"
#include "Source/Shader.cpp"

#include "Header/Tools.h"

int main() {
#pragma region PrepareWindow
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL ) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, initialViewportWidth, initialViewportHeight);
    // glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    // 我们实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
#pragma endregion

#pragma region Prepare Class Instance
    // CallbackExtensions callback_extensions;
#pragma endregion 

#pragma region Init Data
    // Vertices Data Part
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    GLuint VBO[1];
    glGenBuffers(1, VBO);

    glBindVertexArray(VAO[0]);
    // 0. 复制顶点数组到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 1. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Shader Part
    Shader ShaderProgram(vertexShaderSource, fragmentShaderSource);
#pragma endregion 
    
#pragma region Register Callback Function
    glfwSetFramebufferSizeCallback(window, CallbackExtensions::FramebufferSizeCallback);
#pragma endregion

#pragma region Render Loop
    while (!glfwWindowShouldClose(window)) {
        // 输入
        CallbackExtensions::ProcessInput(window);

        // 渲染指令
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f))
        GLCall(glClear(GL_COLOR_BUFFER_BIT))
        
        GLCall(ShaderProgram.use())

        // 查询uniform地址不要求你之前使用过着色器程序，但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        std::cout << greenValue << std::endl;
        int vertexColorLocation = glGetUniformLocation(ShaderProgram.ID, "uniformTestColor");
        GLCall(glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0))

        GLCall(glBindVertexArray(VAO[0]))
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3))

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
