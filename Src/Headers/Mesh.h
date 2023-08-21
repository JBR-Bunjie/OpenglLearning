#pragma once
// #ifndef Mesh
// #define Mesh
#include <glm.hpp>
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // glm::vec3 Tangent;
    // glm::vec3 binormal;
    // glm::vec3 Color;
    // glm::vec4 TEXCOORD0;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader shaderProgram);
private:
    // unsigned int VAO, VBO, EBO;
    unsigned int VBO, EBO;
    void setupMesh();
};

// #endif
