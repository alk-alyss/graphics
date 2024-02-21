#include "scene.hpp"

std::shared_ptr<Portal> Scene::createPortal(const std::shared_ptr<Model> block) const {
    glm::vec3 normalVector = block->getClosestBlockNormal(player->getPosition());

    glm::vec3 portalPosition = block->getPosition() + normalVector * 10.0f;

    return std::make_shared<Portal>(portalPosition, normalVector);
}

void Scene::createFirstPortal(const std::shared_ptr<Model> block) {
    portals.first = createPortal(block);
}

void Scene::createSecondPortal(const std::shared_ptr<Model> block) {
    portals.second = createPortal(block);
}
