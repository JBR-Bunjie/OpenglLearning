#pragma once
#ifndef ConstValues
#define ConstValues

int initialWindowWidth = 800;
int initialWindowHeight = 600;
int initialViewportWidth = 800;
int initialViewportHeight = 600;

bool useGlobalAmbient = false;
glm::vec3 globalAmbientColor = glm::vec3(0.2f, 0.2f, 0.2f);
float globalAmbientStrength = 0.1f;

float specularStrength = 0.5f;
float shininess = 32.0f;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float cubeVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};

float planeVertices[] = {
     // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f, // Origin: 1
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f, // Origin: 3
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f, // Origin: 2

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f, // Origin: 4
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f, // Origin: 6
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f // Origin: 5
    // PLEASE! PAY ATTENTION TO THE FINAL ",". It will cause additional vertex to be generated. Which cause a big problem!
};

float quadVertices[] = {  
 // positions   // texCoords
 -1.0f,  1.0f,  0.0f, 1.0f,
 -1.0f, -1.0f,  0.0f, 0.0f,
  1.0f, -1.0f,  1.0f, 0.0f,

 -1.0f,  1.0f,  0.0f, 1.0f,
  1.0f, -1.0f,  1.0f, 0.0f,
  1.0f,  1.0f,  1.0f, 1.0f
};


float cubeVForSkybox[] = {
 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


float skyboxVertices[] = {
 // positions          
 -1.0f,  1.0f, -1.0f,
 -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,

 -1.0f, -1.0f,  1.0f,
 -1.0f, -1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,
 -1.0f,  1.0f,  1.0f,
 -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

 -1.0f, -1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
 -1.0f, -1.0f,  1.0f,

 -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
 -1.0f,  1.0f, -1.0f,

 -1.0f, -1.0f, -1.0f,
 -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
 -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

// unsigned int indices[] = {
//     0, 1, 3, // 第一个三角形
//     1, 2, 3  // 第二个三角形
// };

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
// For Swizzling
// x y z w ==
// r g b a ==
// s t p q;

const char* BoxVertexShaderSource = "Src\\Shaders\\BoxVertex.glsl";
const char* BoxFragmentShaderSource = "Src\\Shaders\\BoxFragment.glsl";
const char* LightVertexShaderSource = "Src\\Shaders\\LightVertex.glsl";
const char* LightFragmentShaderSource = "Src\\Shaders\\LightFragment.glsl";
// const char* NanosuitVertexShaderSource = "Src\\Shaders\\NanosuitVertex.glsl";
// const char* NanosuitFragmentShaderSource = "Src\\Shaders\\NanosuitFragment.glsl";
const char* NanosuitVertexShaderSource = "E:\\Project\\Engine\\JustTest\\Src\\Shaders\\NanosuitVertex.glsl";
const char* NanosuitFragmentShaderSource = "E:\\Project\\Engine\\JustTest\\Src\\Shaders\\NanosuitFragment.glsl";
const char* stencilTestVertexShaderSource = "Src\\Shaders\\stencil_testing_vert.glsl";
const char* stencilTestFragmentShaderSource = "Src\\Shaders\\stencil_testing_frag.glsl";
const char* stencilSingleColorFragmentShaderSource = "Src\\Shaders\\stencil_single_color_frag.glsl";
const char* offscreenVertexShaderSource = "Src\\Shaders\\offscreenVertex.glsl";
const char* offscreenFragmentShaderSource = "Src\\Shaders\\offscreenFragment.glsl";
const char* skyboxVertexShaderSource = "Src\\Shaders\\skyboxVertex.glsl";
const char* skyboxFragmentShaderSource = "Src\\Shaders\\skyboxFragment.glsl";
const char* box_skyboxVertexShaderSource = "Src\\Shaders\\box_skyboxVertex.glsl";
const char* box_skyboxFragmentShaderSource = "Src\\Shaders\\box_skyboxFragment.glsl";

const char* wallTexSource = "Resources\\Textures\\wall.jpg";
const char* containerTexSource = "Resources\\Textures\\container.jpg";
const char* faceTexSource = "Resources\\Textures\\awesomeface.png";
const char* container2TexSource = "Resources\\Textures\\container2.png";
const char* container2SpecTexSource = "Resources\\Textures\\container2_specular.png";
const char* cubeTexSource = "Resources\\Textures\\marble.jpg";
const char* floorTexSource = "Resources\\Textures\\metal.png";
const char* windowTesSource = "Resources\\Textures\\window.png";
const char* grassTesSource = "Resources\\Textures\\grass.png";

const char* skyboxTexbackSource = "Resources\\Textures\\skybox\\back.jpg";
const char* skyboxTexfrontSource = "Resources\\Textures\\skybox\\front.jpg";
const char* skyboxTexleftSource = "Resources\\Textures\\skybox\\left.jpg";
const char* skyboxTexrightSource = "Resources\\Textures\\skybox\\right.jpg";
const char* skyboxTextopSource = "Resources\\Textures\\skybox\\top.jpg";
const char* skyboxTexbottomSource = "Resources\\Textures\\skybox\\bottom.jpg";

// const char *NanosuitModelSource = "Resources\\Models\\nanosuit\\nanosuit.obj";
const char *NanosuitModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\nanosuit\\nanosuit.obj";
const char *BackpackModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\backpack\\backpack.obj";
#endif