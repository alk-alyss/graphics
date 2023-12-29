#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include "player.hpp"
#include "model.hpp"
#include "light.hpp"

struct Scene {
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Model>> models;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;

    Scene(
        std::shared_ptr<Player> player,
        std::vector<std::shared_ptr<Model>> models,
        std::vector<DirectionalLight> directionalLights,
        std::vector<PointLight> pointLights = std::vector<PointLight>()
    ): player(player), models(models), directionalLights(directionalLights), pointLights(pointLights) {};
};

#endif
