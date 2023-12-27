#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include "model.hpp"
#include "light.hpp"

struct Scene {
    std::vector<std::shared_ptr<Model>> models;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;

    Scene(
        std::vector<std::shared_ptr<Model>> models,
        std::vector<DirectionalLight> directionalLights,
        std::vector<PointLight> pointLights = std::vector<PointLight>()
    ): models(models), directionalLights(directionalLights), pointLights(pointLights) {};
};

#endif
