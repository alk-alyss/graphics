#include "maze.hpp"
#include "common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "material.hpp"
#include <cstdlib>
#include <memory>

using namespace std;

bool isWithinMaze(int x, int y, int width, int height) {
    return x > 0 && x < width-1 && y > 0 && y < height-1;
}

bool isWall(int x, int y, const set<pair<int, int>>& mazeMap) {
    return mazeMap.find(pair<int, int> (x, y)) != mazeMap.end();
}

bool isBorder(int x, int y, int width, int height) {
    return x == 0 || x == width-1 || y == 0 || y == height-1;
}

set<pair<int, int>> neighbours(int x, int y, int width, int height) {
    set<pair<int, int>> neighbours;

    // Moore neighbourhood
    for (int i=-2; i<=2; i+=2) {
        for (int j=-2; j<=2; j+=2) {
            if (i == 0 && j == 0) continue; // skip center
            if (i != 0 && j != 0) continue; // skip diagonals

            pair<int, int> neighbour = pair<int, int> (x+i, y+j);

            if (isWithinMaze(neighbour.first, neighbour.second, width, height)) {
                neighbours.insert(neighbour);
            }
        }
    }

    return neighbours;
}

set<pair<int, int>> wallNeighbours(int x, int y, int width, int height, const set<pair<int, int>>& mazeMap) {
    set<pair<int, int>> frontier = neighbours(x, y, width, height);

    for (auto& neighbour : frontier) {
        if (!isWall(neighbour.first, neighbour.second, mazeMap)) {
            frontier.erase(neighbour);
        }
    }

    return frontier;
}

set<pair<int, int>> passageNeighbours(int x, int y, int width, int height, const set<pair<int, int>>& mazeMap) {
    set<pair<int, int>> frontier = neighbours(x, y, width, height);

    for (auto& neighbour : frontier) {
        if (isWall(neighbour.first, neighbour.second, mazeMap)) {
            frontier.erase(neighbour);
        }
    }

    return frontier;
}

// Prim Maze Generation Algorithm
set<pair<int, int>> generateMazeMap(int width, int height) {
    set<pair<int, int>> mazeMap;

    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
            mazeMap.insert(pair<int, int> (i, j));
        }
    }

    set<pair<int, int>> frontier;

    int x = width/2;
    int y = height/2;
    mazeMap.erase(pair<int, int> (x, y));

    frontier.merge(wallNeighbours(x, y, width, height, mazeMap));

    while (!frontier.empty()) {
        pair<int, int> frontierCell = *next(frontier.begin(), rand() % frontier.size());
        frontier.erase(frontierCell);
        mazeMap.erase(frontierCell);

        x = frontierCell.first;
        y = frontierCell.second;

        auto neighboursSet = passageNeighbours(x, y, width, height, mazeMap);

        if (!neighboursSet.empty()) {
            pair<int, int> neighbour = *next(neighboursSet.begin(), rand() % neighboursSet.size());

            int nx = neighbour.first;
            int ny = neighbour.second;

            mazeMap.erase(pair<int, int> (x + (nx - x)/2, y + (ny - y)/2));
            mazeMap.erase(neighbour);

            auto newNeighboursSet = wallNeighbours(x, y, width, height, mazeMap);
            frontier.merge(newNeighboursSet);
        }
    }

    mazeMap.erase(pair<int, int> (0, 1));
    mazeMap.erase(pair<int, int> (width-1, height-2));

    return mazeMap;
}

Maze::Maze(int width, int height, vector<Material> materials) {
    this->width = width;
    this->height = height;
    this->materials = materials;

    for (size_t i=0; i<materials.size(); i++) {
        modelMatrices.push_back(vector<glm::mat4>());
    }

    int windowProbability = 5;

    // Calculate model matrix for each wall
    for (auto& wall: generateMazeMap(width, height)) {
        mazeMap[wall] = rand() % materials.size();

        // Randomly add windows to walls
        float windowOffset = 0;
        if (rand() % windowProbability == 0 && !isBorder(wall.first, wall.second, width, height)) {
            windowOffset = -scalling/2;
        }

        shared_ptr<Model> wallModel = make_shared<Model>(mesh, materials[mazeMap[wall]]);
        wallModel->setPosition(scalling * wall.second, scalling/2 + windowOffset, -scalling * wall.first);
        wallModel->setScale(scalling/2);

        colliders.push_back(wallModel);
        modelMatrices[mazeMap[wall]].push_back(wallModel->modelMatrix());
    }

    loadVram();
}

void Maze::loadVram() {
    glBindVertexArray(mesh->getVao());

    // Generate model matrix buffer
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // Setup instance matrix attributes
    for (int i=0; i<4; i++) {
        glVertexAttribPointer(INSTANCE_MATRIX_LOCATION+i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(INSTANCE_MATRIX_LOCATION+i);
        glVertexAttribDivisor(INSTANCE_MATRIX_LOCATION+i, 1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Maze::unloadVram() {
    glDeleteBuffers(1, &instanceVBO);
}

void Maze::draw(glm::mat4 modelMatrix, const shared_ptr<Shader> shader) const {
    glUniformMatrix4fv(shader->getMLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glBindVertexArray(mesh->getVao());
    for (size_t i=0; i<materials.size(); i++) {
        materials[i].bind();

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, modelMatrices[i].size() * sizeof(glm::mat4), modelMatrices[i].data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0, modelMatrices[i].size());
    }
    glBindVertexArray(0);
}
