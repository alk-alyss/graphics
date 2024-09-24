#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include <array>

#include "maze.hpp"
#include "player.hpp"
#include "model.hpp"
#include "light.hpp"
#include "portal.hpp"

struct Scene {
    std::shared_ptr<Player> player;
    std::shared_ptr<Maze> maze;
    std::shared_ptr<Model> floor;
    std::vector<std::shared_ptr<Model>> colliders;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;
    std::array<std::shared_ptr<Portal>,2> portals;

public:
    Scene(
        std::shared_ptr<Player> player,
        std::shared_ptr<Maze> maze,
        std::shared_ptr<Model> floor,
        std::vector<std::shared_ptr<Model>> colliders,
        std::vector<DirectionalLight> directionalLights,
        std::vector<PointLight> pointLights = std::vector<PointLight>()
    ):
        player(player),
        maze(maze),
        floor(floor),
        colliders(colliders),
        directionalLights(directionalLights),
        pointLights(pointLights) {};

    void createPortal(int portalIndex);
    std::shared_ptr<Model> castRay(const glm::vec3 position, const glm::vec3 direction, glm::vec3& normalVector);
};

#endif
