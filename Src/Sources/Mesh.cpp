#include "../Headers/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    
    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 0. Set up VAO for current Context
    glBindVertexArray(VAO);

    // 1. Set up VBO, which is the most important thing
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // 2. EBO part
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 3. 指针开始选取，一个VAO下一般存在约16个指针位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, binormal));
    // glEnableVertexAttribArray(4);

    // 4. 解绑VAO
    glBindVertexArray(0);
}

void Mesh::Draw(Shader shaderProgram) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        string number;
        string name = textures[i].type;
    
        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if (name == "texture_specular") number = std::to_string(specularNr++);
        else if (name == "texture_normal") number = std::to_string(normalNr++);
        else if (name == "texture_height") number = std::to_string(heightNr++);
    
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id))
        // shaderProgram.setFloat(("material." + name + number).c_str(), i);
        // GLCall(shaderProgram.setFloat((name + number).c_str(), i))
        // GLCall(shaderProgram.setInt((name + number).c_str(), i))
        // cout << name << number << endl;
        // uniformName = name + number;
        // GLCall(shaderProgram.setInt(name + number, i))
        glUniform1i(glGetUniformLocation(shaderProgram.ID, (name + number).c_str()), i);
    }

    GLCall(glBindVertexArray(VAO))
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0))
    GLCall(glBindVertexArray(0))

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}