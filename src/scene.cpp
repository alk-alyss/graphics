#include "scene.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::shared_ptr<Portal> Scene::createPortal(
        const std::shared_ptr<Model> block,
        const glm::vec3 normalVector
    ) const {
    glm::vec3 portalPosition = block->getPosition() + normalVector * (block->getSize() + 0.005f);

    return std::make_shared<Portal>(portalPosition, normalVector, 0.8f * block->getSize());
}

void Scene::createFirstPortal(const std::shared_ptr<Model> block, glm::vec3 normalVector) {
    portals.first = createPortal(block, normalVector);
}

void Scene::createSecondPortal(const std::shared_ptr<Model> block, glm::vec3 normalVector) {
    portals.second = createPortal(block, normalVector);
}
