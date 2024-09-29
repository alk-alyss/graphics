#ifndef MESH_HPP
#define MESH_HPP

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common.hpp"
#include "shader.hpp"

static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAUTL_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAUTL_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;

    Vertex() {};

    Vertex(
        const glm::vec3& position,
        const glm::vec2& uv,
        const glm::vec3& normal,
        const glm::vec3& tangent = glm::vec3(0)
    ) {
        this->position = position;
        this->uv = uv;
        this->normal = normal;
        this->tangent = tangent;
    }

    bool operator<(const Vertex that) const {
        return memcmp((void*) this, (void*) &that, sizeof(Vertex)) > 0;
    };
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(const glm::vec3 min, const glm::vec3 max): min(min), max(max) {};
    AABB(const std::vector<glm::vec3>& vertices);
    AABB(const std::vector<Vertex>& vertices);

    bool intersects(const AABB& other) const ;
    bool intersects(const glm::vec3 point) const ;
};

class Mesh {
protected:
    std::vector<Vertex> indexedVertices;
    std::vector<unsigned int> indices;

    GLuint VAO, VBO, EBO;

    AABB aabb;

public:
    Mesh(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& uvs,
        const std::vector<glm::vec3>& normals
    );
    Mesh(const std::string filename);
    ~Mesh() {unloadVram();}

    //Delete the copy constructor/assignment.
    Mesh(const Mesh&) = delete;
    Mesh &operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) :
        indexedVertices(other.indexedVertices),
        indices(other.indices),
        VAO(other.VAO),
        VBO(other.VBO),
        EBO(other.EBO),
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
            std::swap(VBO, other.VBO);
            std::swap(EBO, other.EBO);
            std::swap(indexedVertices, other.indexedVertices);
            std::swap(indices, other.indices);
            std::swap(aabb, other.aabb);
        }

        return *this;
    }

    void loadVram();
    void unloadVram();

    const AABB getAABB() const {return aabb;}

    int indexCount() const {return indices.size();}
    int vertexCount() const {return indexedVertices.size();}
    GLuint getVao() const {return VAO;}

    void draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const;

    static std::shared_ptr<Mesh> plane();
};

#endif
