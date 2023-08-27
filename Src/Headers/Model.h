#pragma once
#ifndef Model
// #define Model

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    void Draw(Shader shaderProgram);
    Model(const char *path);
    std::vector<Mesh> meshes;
private: 
    string directory;
    std::vector<Texture> textures_loaded;
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};


static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false) {
    // string filename = string(path);
    // filename = directory + '/' + filename;
    string filename = directory + '/' + string(path);
    
    unsigned int textureID = RegisterTexture(filename.c_str());

    return textureID;
}

#endif
