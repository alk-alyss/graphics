#include "mesh.h"

Mesh::Mesh(std::string filename) {

}

void Mesh::loadVram() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
}

void Mesh::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size()/3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::unloadVram() {
    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}