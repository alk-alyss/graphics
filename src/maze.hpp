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

    std::set<std::pair<int, int>> mazeMap;

    std::shared_ptr<Mesh> mesh = cubeMesh;
    Material material;

    GLuint instanceVBO;

    std::vector<glm::mat4> modelMatrices;

    void loadVram();
    void unloadVram();

public:
    Maze(int width, int height, Material material);

    ~Maze() {
        unloadVram();
    }

    void draw(glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const override;
};

#endif
