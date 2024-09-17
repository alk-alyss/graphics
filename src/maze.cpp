#include "maze.hpp"
#include "common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdlib>
#include <memory>

using namespace std;

bool isWithinMaze(int x, int y, int width, int height) {
    return x > 0 && x < width-1 && y > 0 && y < height-1;
}

bool isWall(int x, int y, set<pair<int, int>> mazeMap) {
    return mazeMap.find(pair<int, int> (x, y)) != mazeMap.end();
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

set<pair<int, int>> wallNeighbours(int x, int y, int width, int height, set<pair<int, int>> mazeMap) {
    set<pair<int, int>> frontier = neighbours(x, y, width, height);

    for (auto& neighbour : frontier) {
        if (!isWall(neighbour.first, neighbour.second, mazeMap)) {
            frontier.erase(neighbour);
        }
    }

    return frontier;
}

set<pair<int, int>> passageNeighbours(int x, int y, int width, int height, set<pair<int, int>> mazeMap) {
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

Maze::Maze(int width, int height, Material material) {
    this->width = width;
    this->height = height;
    this->material = material;

    mazeMap = generateMazeMap(width, height);

    // Calculate model matrix for each wall
    for (auto& wall: mazeMap) {
        Transformation wallTransformation;
        wallTransformation.setPosition(scalling * wall.second, scalling/2, -scalling * wall.first);
        wallTransformation.setScale(scalling/2);

        modelMatrices.push_back(wallTransformation.modelMatrix());
    }

    loadVram();
}

void Maze::loadVram() {
    // Calculate model matrix for each wall
    glBindVertexArray(mesh->getVao());

    // Generate model matrix buffer
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // Load model matrix for each instance
    for (int i=0; i<4; i++) {
        glVertexAttribPointer(INSTANCE_MATRIX_LOCATION+i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(INSTANCE_MATRIX_LOCATION+i);
        glVertexAttribDivisor(INSTANCE_MATRIX_LOCATION+i, 1);
    }

    glBindVertexArray(0);

    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(modelMatrices[0]), modelMatrices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Maze::unloadVram() {
    glDeleteBuffers(1, &instanceVBO);
}

void Maze::draw(glm::mat4 modelMatrix, const shared_ptr<Shader> shader) const {
    glUniformMatrix4fv(shader->getMLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    material.bind();

    glBindVertexArray(mesh->getVao());
    glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0, mazeMap.size());
    glBindVertexArray(0);
}
