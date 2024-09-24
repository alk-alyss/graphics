#include "scene.hpp"

#include <glm/gtx/string_cast.hpp>

void Scene::createPortal(const int portalIndex) {
    glm::vec3 normalVector;
    auto block = castRay(player->getPosition(), player->forward(), normalVector);
    if (block == nullptr) return;

    glm::vec3 portalPosition = block->getPosition() + normalVector * (block->getScale() + 0.005f);

    if (portalPosition.y == 0) return;

    for (auto& portal : portals) {
        if (portal != nullptr && portal->getPosition() == portalPosition) return;
    }

    portals[portalIndex] = std::make_shared<Portal>(portalPosition, normalVector, 0.8f * block->getScale());

    for (int i=0; i<portals.size(); i++) {
        if (portals[i] == nullptr) continue;
        portals[i]->linkPortal(portals[(i + 1) % portals.size()]);
    }
}

std::shared_ptr<Model> Scene::castRay(
    const glm::vec3 position,
    const glm::vec3 direction,
    glm::vec3& normalVector
) {
    glm::vec3 rayPosition = position;
    float stepSize = 0.2;
    glm::vec3 rayDirection = glm::normalize(direction) * stepSize;

    int stepLimit = 200;
    for (int i=0; i<stepLimit; i++) {
        for (auto& collider : colliders) {
            AABB colliderAABB = collider->getAABB();

            if (colliderAABB.intersects(rayPosition)) {
                normalVector = collider->getClosestBlockNormal(rayPosition);

                return collider;
            }
        }

        rayPosition += rayDirection;
    }

    return nullptr;
}
