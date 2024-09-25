#include "mesh.hpp"
#include "common.hpp"

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
    vector<Vertex>& vertices,
    vector<unsigned int>& indices
) {
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;

    string err, warn;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw runtime_error(err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vec3 position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vec2 uv = vec2(0);
            if (attrib.texcoords.size() != 0) {
                uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            vec3 normal = vec3(0);
            if (attrib.normals.size() != 0) {
                normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            Vertex vertex = {position, uv, normal};

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    // TODO .mtl loader
}

void calculateTangents(
    vector<Vertex>& vertices,
    const vector<unsigned int>& indices
){
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i+1]];
        Vertex& v2 = vertices[indices[i+2]];

        vec3 edge1 = v1.position - v0.position;
        vec3 edge2 = v2.position - v0.position;

        vec2 deltaUV1 = v1.uv - v0.uv;
        vec2 deltaUV2 = v2.uv - v0.uv;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        vec3 tangent = {
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;
    }

    for (auto& vertex : vertices) {
        vertex.tangent = normalize(vertex.tangent);
    }
}

bool getSimilarVertexIndex(
    const Vertex& packed,
    map<Vertex, unsigned int>& vertexToOutIndex,
    unsigned int& result
) {
    map<Vertex, unsigned int>::iterator it = vertexToOutIndex.find(packed);
    if (it == vertexToOutIndex.end()) {
        return false;
    } else {
        result = it->second;
        return true;
    }
}

void indexVBO(
    const vector<Vertex>& in_vertices,
    vector<unsigned int>& out_indices,
    vector<Vertex>& out_vertices
) {
    map<Vertex, unsigned int> vertexToOutIndex;

    for (const auto& vertex: in_vertices) {
        // Try to find a similar vertex in out_XXXX
        unsigned int index;
        bool found = getSimilarVertexIndex(vertex, vertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back(index);
        } else { // If not, it needs to be added in the output data.
            out_vertices.push_back(vertex);
            unsigned int newindex = (unsigned int) out_vertices.size() - 1;
            out_indices.push_back(newindex);
            vertexToOutIndex[vertex] = newindex;
        }
    }
}

AABB::AABB(const std::vector<glm::vec3>& vertices) {
    float xmin = vertices[0].x, xmax = xmin;
    float ymin = vertices[0].y, ymax = ymin;
    float zmin = vertices[0].z, zmax = zmin;

    for (auto& vertex : vertices) {
        xmin = std::min(vertex.x, xmin);
        ymin = std::min(vertex.y, ymin);
        zmin = std::min(vertex.z, zmin);

        xmax = std::max(vertex.x, xmax);
        ymax = std::max(vertex.y, ymax);
        zmax = std::max(vertex.z, zmax);
    }

    min = glm::vec3(xmin, ymin, zmin);
    max = glm::vec3(xmax, ymax, zmax);
}

AABB::AABB(const std::vector<Vertex>& vertices) {
    float xmin = vertices[0].position.x, xmax = xmin;
    float ymin = vertices[0].position.y, ymax = ymin;
    float zmin = vertices[0].position.z, zmax = zmin;

    for (auto& vertex : vertices) {
        xmin = std::min(vertex.position.x, xmin);
        ymin = std::min(vertex.position.y, ymin);
        zmin = std::min(vertex.position.z, zmin);

        xmax = std::max(vertex.position.x, xmax);
        ymax = std::max(vertex.position.y, ymax);
        zmax = std::max(vertex.position.z, zmax);
    }

    min = glm::vec3(xmin, ymin, zmin);
    max = glm::vec3(xmax, ymax, zmax);
};

bool AABB::intersects(const AABB& other) {
    return this->min.x <= other.max.x &&
        this->max.x >= other.min.x &&
        this->min.y <= other.max.y &&
        this->max.y >= other.min.y &&
        this->min.z <= other.max.z &&
        this->max.z >= other.min.z;
}

bool AABB::intersects(const glm::vec3 point) {
    return point.x >= this->min.x &&
        point.x <= this->max.x &&
        point.y >= this->min.y &&
        point.y <= this->max.y &&
        point.z >= this->min.z &&
        point.z <= this->max.z;
}

Mesh::Mesh(
    const vector<vec3>& vertices,
    const vector<vec2>& uvs,
    const vector<vec3>& normals
) {
    for (size_t i=0; i<vertices.size(); i++) {
        Vertex vertex = {vertices[i], uvs[i], normals[i]};
        this->vertices.push_back(vertex);
    }

    aabb = AABB(this->vertices);
    indexVBO(this->vertices, indices, indexedVertices);
    calculateTangents(this->indexedVertices, indices);
    loadVram();
}

Mesh::Mesh(std::string path) {
    if (path.substr(path.size() - 3, 3) == "obj") {
        loadOBJWithTiny(path.c_str(), vertices, VEC_UINT_DEFAUTL_VALUE);
    } else {
        throw runtime_error("File format not supported: " + path);
    }

    aabb = AABB(vertices);
    indexVBO(vertices, indices, indexedVertices);
    calculateTangents(indexedVertices, indices);
    loadVram();
}

void Mesh::loadVram() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(indexedVertices[0]), indexedVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glEnableVertexAttribArray(POSITION_LOCATION);

    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(NORMAL_LOCATION);

    glVertexAttribPointer(UV_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(UV_LOCATION);

    glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(TANGENT_LOCATION);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::unloadVram() {
    glDeleteBuffers(1, &verticesVBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const {
    glUniformMatrix4fv(shader->getMLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));

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
