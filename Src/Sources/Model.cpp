#include "../Headers/Model.h"

Model::Model(const char* path) {
    loadModel(path);
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('\\'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(Model::processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        Model::processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // 处理顶点位置、法线和纹理坐标
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec2 tempVec2BufferVector;
        glm::vec3 tempVec3BufferVector;
        tempVec3BufferVector.x = mesh->mVertices[i].x;
        tempVec3BufferVector.y = mesh->mVertices[i].y;
        tempVec3BufferVector.z = mesh->mVertices[i].z;
        vertex.Position = tempVec3BufferVector;

        tempVec3BufferVector.x = mesh->mNormals[i].x;
        tempVec3BufferVector.y = mesh->mNormals[i].y;
        tempVec3BufferVector.z = mesh->mNormals[i].z;
        vertex.Normal = tempVec3BufferVector;

        // if (mesh->mTangents) {
        //     tempVec3BufferVector.x = mesh->mTangents[i].x;
        //     tempVec3BufferVector.y = mesh->mTangents[i].y;
        //     tempVec3BufferVector.z = mesh->mTangents[i].z;
        //     vertex.Tangent = tempVec3BufferVector;
        // }

        // tempVec3BufferVector.x = mesh->mBitangents[i].x;
        // tempVec3BufferVector.y = mesh->mBitangents[i].y;
        // tempVec3BufferVector.z = mesh->mBitangents[i].z;
        // vertex.Bitangent = tempVec3BufferVector;

        if (mesh->mTextureCoords[0]) {
            tempVec2BufferVector.x = mesh->mTextureCoords[0][i].x;
            tempVec2BufferVector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = tempVec2BufferVector;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Indices
    // 对应mMeshes[i]->mFaces[j],实际类同于Unity的mesh.triangles: https://docs.unity3d.com/ScriptReference/Mesh-triangles.html
    /*  The mesh face data, i.e. the triangles it is made of, is simply three vertex indices for each triangle.
        For example, if the mesh has 10 triangles, then the triangles array should be 30 numbers,
        with each number indicating which vertex to use.
        The first three elements in the triangles array are the indices for the vertices that make up that triangle;
        the second three elements make up another triangle and so on.
        —— From: https://docs.unity3d.com/ScriptReference/Mesh.html */
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Materials
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            // if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
            if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

void Model::Draw(Shader shaderProgram) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shaderProgram);
    }
}