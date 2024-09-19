#include "scene.hpp"

#include <glm/gtx/string_cast.hpp>

std::shared_ptr<Portal> Scene::createPortal(
        const std::shared_ptr<Model> block,
        const glm::vec3 normalVector
    ) const {
    glm::vec3 portalPosition = block->getPosition() + normalVector * (block->getSize() + 0.005f);

    return std::make_shared<Portal>(portalPosition, normalVector, 0.8f * block->getSize());
}

void Scene::linkPortals() {
    if (portals[0] != nullptr) portals[0]->linkPortal(portals[1]);
    if (portals[1] != nullptr) portals[1]->linkPortal(portals[0]);
}

void Scene::createFirstPortal(const std::shared_ptr<Model> block, glm::vec3 normalVector) {
    portals[0] = createPortal(block, normalVector);
    linkPortals();
}

void Scene::createSecondPortal(const std::shared_ptr<Model> block, glm::vec3 normalVector) {
    portals[1] = createPortal(block, normalVector);
    linkPortals();
}
