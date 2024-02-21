#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include "player.hpp"
#include "model.hpp"
#include "light.hpp"
#include "portal.hpp"

struct Scene {
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Model>> models;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;
    std::pair<std::shared_ptr<Portal>, std::shared_ptr<Portal>> portals;

private:
    std::shared_ptr<Portal> createPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector) const;

public:
    Scene(
        std::shared_ptr<Player> player,
        std::vector<std::shared_ptr<Model>> models,
        std::vector<DirectionalLight> directionalLights,
        std::vector<PointLight> pointLights = std::vector<PointLight>()
    ): player(player), models(models), directionalLights(directionalLights), pointLights(pointLights) {};

    void createFirstPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector);
    void createSecondPortal(const std::shared_ptr<Model> block, const glm::vec3 normalVector);
};

#endif
