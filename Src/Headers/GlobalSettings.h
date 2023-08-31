#pragma once
#ifndef ConstValues
#define ConstValues

int windowWidth = 800;
int windowHeight = 600;
int viewportWidth = 800;
int viewportHeight = 600;

const float PI = 3.14159265358979323846264338327950288f;
const float Rad2Deg = 180.0f / PI;
const float Deg2Rad = PI / 180.0f;

const unsigned int SHADOW_WIDTH = 1024.0f;
const unsigned int SHADOW_HEIGHT = 1024.0f;


bool useGlobalAmbient = false;
glm::vec3 globalAmbientColor = glm::vec3(0.2f, 0.2f, 0.2f);
float globalAmbientStrength = 0.1f;

float specularStrength = 0.5f;
float shininess = 32.0f;

    
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
const char* GeometryShaderVertexShaderSource = "Src\\Shaders\\GeometryShaderVertex.glsl";
const char* GeometryShaderFragmentShaderSource = "Src\\Shaders\\GeometryShaderFragment.glsl";
const char* GeometryShaderGeometryShaderSource = "Src\\Shaders\\GeometryShaderGeometry.glsl";
const char* GeometryVisualizedNormalVertexShaderSource = "Src\\Shaders\\GeometryVisualizedNormalVertex.glsl";
const char* GeometryVisualizedNormalFragmentShaderSource = "Src\\Shaders\\GeometryVisualizedNormalFragment.glsl";
const char* GeometryVisualizedNormalGeometryShaderSource = "Src\\Shaders\\GeometryVisualizedNormalGeometry.glsl";
const char* InstancePlanetVertexShaderSource = "Src\\Shaders\\InstancePlanetVertex.glsl";
const char* InstancePlanetFragmentShaderSource = "Src\\Shaders\\InstancePlanetFragment.glsl";
const char* InstanceAsteroidVertexShaderSource = "Src\\Shaders\\InstanceAsteroidVertex.glsl";
const char* InstanceAsteroidFragmentShaderSource = "Src\\Shaders\\InstanceAsteroidFragment.glsl";
const char* ShadowMap_Directional_Generate_Vertex = "Src\\Shaders\\ShadowMap_Directional_Generate_Vertex.glsl";
const char* ShadowMap_Directional_Generate_Fragment = "Src\\Shaders\\ShadowMap_Directional_Generate_Fragment.glsl";
const char* ShadowMap_Directional_Apply_Vertex = "Src\\Shaders\\ShadowMap_Directional_Apply_Vertex.glsl";
const char* ShadowMap_Directional_Apply_Fragment = "Src\\Shaders\\ShadowMap_Directional_Apply_Fragment.glsl";
const char* ShadowMap_Point_Generate_Vertex = "Src\\Shaders\\ShadowMap_Point_Generate_Vertex.glsl";
const char* ShadowMap_Point_Generate_Fragment = "Src\\Shaders\\ShadowMap_Point_Generate_Fragment.glsl";
const char* ShadowMap_Point_Generate_Geometry = "Src\\Shaders\\ShadowMap_Point_Generate_Geometry.glsl";
const char* ShadowMap_Point_Apply_Vertex = "Src\\Shaders\\ShadowMap_Point_Apply_Vertex.glsl";
const char* ShadowMap_Point_Apply_Fragment = "Src\\Shaders\\ShadowMap_Point_Apply_Fragment.glsl";


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
const char* woodTexSource = "Resources\\Textures\\wood.png";

const char* NanosuitModelSource = "Resources\\Models\\nanosuit\\nanosuit.obj";
// const char* NanosuitModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\nanosuit\\nanosuit.obj";
const char* BackpackModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\backpack\\backpack.obj";
const char* PlanetModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\planet\\planet.obj";
const char* RockModelSource = "E:\\Project\\Engine\\JustTest\\Resources\\Models\\rock\\rock.obj";

#endif