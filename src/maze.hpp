#ifndef MAZE_HPP
#define MAZE_HPP

#include "material.hpp"
#include "mesh.hpp"
#include "resources.hpp"
#include <memory>
#include <set>

std::set<std::pair<int, int>> generateMazeMap(int width, int height);

class Maze : Node{
private:
    int width;
    int height;
    float scalling = 3;

    std::map<std::pair<int, int>, int> mazeMap; // location -> material index

    std::shared_ptr<Mesh> mesh = cubeMesh;
    std::vector<Material> materials;

    GLuint instanceVBO;

    std::vector<std::vector<glm::mat4>> modelMatrices; // material index -> model matrices

    void loadVram();
    void unloadVram();

public:
    Maze(int width, int height, std::vector<Material> materials);

    ~Maze() {unloadVram();}

    void draw(glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const override;
};

#endif
