#include "mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <cstring>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace std;
using namespace glm;

void loadOBJWithTiny(
    const string& path,
    vector<vec3>& vertices,
    vector<vec2>& uvs,
    vector<vec3>& normals,
    vector<unsigned int>& indices) {
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;

    string err, warn;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw runtime_error(err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vec3 vertex = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};
            if (attrib.texcoords.size() != 0) {
                vec2 uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]};
                uvs.push_back(uv);
            }
            if (attrib.normals.size() != 0) {
                vec3 normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]};
                normals.push_back(normal);
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    // TODO .mtl loader
}

struct PackedVertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    bool operator<(const PackedVertex that) const {
        return memcmp((void*) this, (void*) &that, sizeof(PackedVertex)) > 0;
    };
};

bool getSimilarVertexIndex(
    PackedVertex& packed,
    map<PackedVertex, unsigned int>& vertexToOutIndex,
    unsigned int& result) {
    map<PackedVertex, unsigned int>::iterator it = vertexToOutIndex.find(packed);
    if (it == vertexToOutIndex.end()) {
        return false;
    } else {
        result = it->second;
        return true;
    }
}

void indexVBO(
    const vector<vec3>& in_vertices,
    const vector<vec2>& in_uvs,
    const vector<vec3>& in_normals,
    vector<unsigned int>& out_indices,
    vector<vec3>& out_vertices,
    vector<vec2>& out_uvs,
    vector<vec3>& out_normals) {
    map<PackedVertex, unsigned int> vertexToOutIndex;

    // For each input vertex
    for (int i = 0; i < static_cast<int>(in_vertices.size()); i++) {
        vec3 vertices = in_vertices[i];
        vec2 uvs;
        vec3 normals;
        if (in_uvs.size() != 0) uvs = in_uvs[i];
        if (in_normals.size() != 0) normals = in_normals[i];
        PackedVertex packed = {vertices, uvs, normals};

        // Try to find a similar vertex in out_XXXX
        unsigned int index;
        bool found = getSimilarVertexIndex(packed, vertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back(index);
        } else { // If not, it needs to be added in the output data.
            out_vertices.push_back(vertices);
            if (in_uvs.size() != 0) out_uvs.push_back(uvs);
            if (in_normals.size() != 0) out_normals.push_back(normals);
            unsigned int newindex = (unsigned int) out_vertices.size() - 1;
            out_indices.push_back(newindex);
            vertexToOutIndex[packed] = newindex;
        }
    }
}

Mesh::Mesh(
    const vector<vec3>& vertices,
    const vector<vec2>& uvs,
    const vector<vec3>& normals
) : vertices(vertices), uvs(uvs), normals(normals) {
    indexVBO(vertices, uvs, normals, indices, indexedVertices, indexedUVS, indexedNormals);
    loadVram();
}

Mesh::Mesh(std::string path) {
    if (path.substr(path.size() - 3, 3) == "obj") {
        loadOBJWithTiny(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else {
        throw runtime_error("File format not supported: " + path);
    }

    indexVBO(vertices, uvs, normals, indices, indexedVertices, indexedUVS, indexedNormals);
    loadVram();
}

void Mesh::loadVram() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(indexedVertices[0]), indexedVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    if (indexedNormals.size() != 0) {
        glGenBuffers(1, &normalsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
        glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(indexedNormals[0]), indexedNormals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }

    if (indexedUVS.size() != 0) {
        glGenBuffers(1, &uvsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
        glBufferData(GL_ARRAY_BUFFER, indexedUVS.size() * sizeof(indexedUVS[0]), indexedUVS.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::unloadVram() {
    glDeleteBuffers(1, &verticesVBO);
    glDeleteBuffers(1, &normalsVBO);
    glDeleteBuffers(1, &uvsVBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(glm::mat4 modelMatrix, Shader& shader) const {
    glUniformMatrix4fv(shader.getMLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::shared_ptr<Mesh> Mesh::plane() {
    vector<vec3> vertices = {
        vec3(-1.0f, 0, -1.0f),
        vec3(-1.0f, 0,  1.0f),
        vec3(1.0f,  0,  1.0f),
        vec3(1.0f,  0,  1.0f),
        vec3(1.0f,  0, -1.0f),
        vec3(-1.0f, 0, -1.0f),

    };

    vector<vec3> normals = {
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f)
    };

    vector<vec2> uvs = {
        vec2(0.0f, 0.0f),
        vec2(0.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 0.0f),
        vec2(0.0f, 0.0f),
    };

    return std::make_shared<Mesh>(vertices, uvs, normals);
}
