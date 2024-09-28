#ifndef MAZE_HPP
#define MAZE_HPP

#include "material.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include <memory>
#include <set>

std::set<std::pair<int, int>> generateMazeMap(int width, int height);

class Maze {
private:
    int width = 19;
    int height = 19;
    float scalling = 3;

    std::map<std::pair<int, int>, int> mazeMap; // location -> material index

    std::shared_ptr<Mesh> mesh = cubeMesh;
    std::vector<Material> materials;

    GLuint instanceVBO;

    std::vector<std::vector<glm::mat4>> modelMatrices; // material index -> model matrices
    std::vector<std::shared_ptr<Model>> colliders;

    void loadVram();
    void unloadVram();

public:
    Maze(const int width, const int height, const std::vector<Material> materials);

    ~Maze() {unloadVram();}

    std::vector<std::shared_ptr<Model>> getColliders() const {return colliders;}
    void draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const;
};

#endif
