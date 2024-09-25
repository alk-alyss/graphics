#include "scene.hpp"
#include "resources.hpp"

#include <glm/gtx/string_cast.hpp>

void Scene::createPortal(const int portalIndex) {
    glm::vec3 normalVector;
    auto block = castRay(player->getPosition(), player->forward(), normalVector);
    if (block == nullptr) return;

    glm::vec3 portalPosition = block->getPosition() + normalVector * (block->getScale() + 0.005f);

    // If block is a window, don't create portal
    if (portalPosition.y == 0) return;

    // If portal would be created inside a block, don't create portal
    for (auto& collider : colliders) {
        if (collider->getAABB().intersects(portalPosition + normalVector * (block->getScale() + 0.005f))) return;
    }

    // If portal already exists at this position, don't create portal
    for (auto& portal : portals) {
        if (portal != nullptr && portal->getPosition() == portalPosition) return;
    }

    Material portalMaterial;
    if (portalIndex == 0) {
        portalMaterial = bluePortalMaterial;
    } else if (portalIndex == 1) {
        portalMaterial = orangePortalMaterial;
    }
    portals[portalIndex] = std::make_shared<Portal>(portalPosition, normalVector, 0.8f * block->getScale(), portalMaterial);

    // Link portals
    for (size_t i=0; i<portals.size(); i++) {
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
