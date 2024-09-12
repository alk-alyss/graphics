#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common.hpp"

static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAUTL_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAUTL_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    Vertex() {};

    Vertex(
        const glm::vec3 position,
        const glm::vec2 uv,
        const glm::vec3 normal
    ) {
        this->position = position;
        this->uv = uv;
        this->normal = normal;
    }

    bool operator<(const Vertex that) const {
        return memcmp((void*) this, (void*) &that, sizeof(Vertex)) > 0;
    };
};

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
    std::vector<unsigned int> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
);

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(glm::vec3 min, glm::vec3 max): min(min), max(max) {};
    AABB(const std::vector<glm::vec3>& vertices);
    AABB(const std::vector<Vertex>& vertices);

    bool intersects(const AABB& other);
    bool intersects(const glm::vec3 point);
};

class Mesh : public Node{
protected:
    std::vector<Vertex> vertices, indexedVertices;
    std::vector<unsigned int> indices;

    GLuint VAO, verticesVBO, EBO;

    AABB aabb;

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
        EBO(other.EBO),
        vertices(other.vertices),
        indexedVertices(other.indexedVertices),
        indices(other.indices),
        aabb(other.aabb)
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
            std::swap(EBO, other.EBO);
            std::swap(vertices, other.vertices);
            std::swap(indexedVertices, other.indexedVertices);
            std::swap(indices, other.indices);
            std::swap(aabb, other.aabb);
        }

        return *this;
    }

    void loadVram();
    void unloadVram();

    const AABB getAABB() {return aabb;}

    int vertexCount() const {return vertices.size();}
    GLuint getVao() const {return VAO;}

    void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const;

    static std::shared_ptr<Mesh> plane();
};

#endif
