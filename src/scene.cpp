#include "scene.hpp"

std::shared_ptr<Portal> Scene::createPortal(const std::shared_ptr<Model> block) const {
    glm::vec3 normalVector = block->getClosestBlockNormal(player->getPosition());

    glm::vec3 portalPosition = block->getPosition() + normalVector * (block->getSize() + 0.001f);

    return std::make_shared<Portal>(portalPosition, normalVector, 0.8f * block->getSize());
}

void Scene::createFirstPortal(const std::shared_ptr<Model> block) {
    portals.first = createPortal(block);
}

void Scene::createSecondPortal(const std::shared_ptr<Model> block) {
    portals.second = createPortal(block);
}
