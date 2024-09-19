#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

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
    std::shared_ptr<Portal> portals[2];

private:
    std::shared_ptr<Portal> createPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector) const;
    void linkPortals();

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

    void createFirstPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector);
    void createSecondPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector);
};

#endif
