#include "mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh() {
    vertices = {
        1.0f,1.0f,1.0f,		// 0
        1.0f,1.0f,-1.0f,	// 1
        1.0f,-1.0f,1.0f,	// 2
        1.0f,-1.0f,-1.0f,	// 3
        -1.0f,1.0f,1.0f,	// 4
        -1.0f,1.0f,-1.0f,	// 5
        -1.0f,-1.0f,1.0f,	// 6
        -1.0f,-1.0f,-1.0f,  // 7
    };

    faces = {
        4,2,0,	// Front Face
        4,6,2,
        0,3,1,	// Right Face
        0,2,3,
        1,7,5,	// Back Face
        1,3,7,
        5,6,4,	// Left Face
        5,7,6,
        4,1,5,	// Top Face
        4,0,1,
        6,3,2,	// Bottom Face
        6,7,3
    };

    colors = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
    };

    loadVram();
}

Mesh::Mesh(std::string filename) {

    loadVram();
}

void Mesh::loadVram() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]), faces.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

glm::mat4 Mesh::modelMatrix() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::translate(modelMatrix, position);

    return modelMatrix;
}

void Mesh::unloadVram() {
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
