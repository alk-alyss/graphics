#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common.hpp"

static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAUTL_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAUTL_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};

void loadOBJWithTiny(
    const std::string& path,
    std::vector<glm::vec3>& vertices,
    std::vector<glm::vec2>& uvs,
    std::vector<glm::vec3>& normals,
    std::vector<unsigned int>& indices = VEC_UINT_DEFAUTL_VALUE
);

void indexVBO(
    const std::vector<glm::vec3>& in_vertices,
    const std::vector<glm::vec2>& in_uvs,
    const std::vector<glm::vec3>& in_normals,
    const std::vector<glm::vec3>& in_tangents,
    const std::vector<glm::vec3>& in_bitangents,
    std::vector<unsigned int> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals,
    std::vector<glm::vec3> & out_tangents,
    std::vector<glm::vec3> & out_bitangents
);

class Mesh : public Node{
protected:
    std::vector<glm::vec3> vertices, normals, tangents, bitangents;
    std::vector<glm::vec3> indexedVertices, indexedNormals, indexedTangents, indexedBitangents;
    std::vector<glm::vec2> uvs, indexedUVS;
    std::vector<unsigned int> indices;

    GLuint VAO;
    GLuint verticesVBO, uvsVBO, normalsVBO, tangentsVBO, bitangentsVBO;
    GLuint EBO;

public:
    Mesh(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec2>& uvs,
        const std::vector<glm::vec3>& normals
    );
    Mesh(std::string filename);
    ~Mesh() {unloadVram();}

    //Delete the copy constructor/assignment.
    Mesh(const Mesh&) = delete;
    Mesh &operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) :
        VAO(other.VAO),
        verticesVBO(other.verticesVBO),
        uvsVBO(other.uvsVBO),
        normalsVBO(other.normalsVBO),
        tangentsVBO(other.tangentsVBO),
        bitangentsVBO(other.bitangentsVBO),
        EBO(other.EBO),
        vertices(other.vertices),
        indexedVertices(other.indexedVertices),
        normals(other.normals),
        indexedNormals(other.indexedNormals),
        tangents(other.tangents),
        indexedTangents(other.indexedTangents),
        bitangents(other.bitangents),
        indexedBitangents(other.indexedBitangents),
        uvs(other.uvs),
        indexedUVS(other.indexedUVS),
        indices(other.indices)
    {
        other.VAO = 0; //Use the "null" VAO for the old object.
    }

    Mesh &operator=(Mesh&& other) {
        //ALWAYS check for self-assignment.
        if(this != &other)
        {
            unloadVram();
            std::swap(VAO, other.VAO);
            std::swap(verticesVBO, other.verticesVBO);
            std::swap(uvsVBO, other.uvsVBO);
            std::swap(normalsVBO, other.normalsVBO);
            std::swap(tangentsVBO, other.tangentsVBO);
            std::swap(bitangentsVBO, other.bitangentsVBO);
            std::swap(EBO, other.EBO);
            std::swap(vertices, other.vertices);
            std::swap(indexedVertices, other.indexedVertices);
            std::swap(normals, other.normals);
            std::swap(indexedNormals, other.indexedNormals);
            std::swap(tangents, other.tangents);
            std::swap(indexedTangents, other.indexedTangents);
            std::swap(bitangents, other.bitangents);
            std::swap(indexedBitangents, other.indexedBitangents);
            std::swap(uvs, other.uvs);
            std::swap(indexedUVS, other.indexedUVS);
            std::swap(indices, other.indices);
        }

        return *this;
    }

    void loadVram();
    void unloadVram();

    int vertexCount() const {return vertices.size();}
    GLuint getVao() const {return VAO;}

    void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const;

    static std::shared_ptr<Mesh> plane();
};

#endif
