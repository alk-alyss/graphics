#include "collisions.hpp"

void handleCollision(const std::shared_ptr<Player> player, const std::shared_ptr<Model> model, const float deltaTime) {
    glm::vec3 normalVector = model->getClosestBlockNormal(player->getPosition());

    glm::vec3 playerVelocity = player->getVelocity();
    glm::vec3 kickbackVector = glm::reflect(playerVelocity, normalVector);

    float speed = glm::dot(normalVector, kickbackVector);
    if (speed < 0) speed = 0;

    kickbackVector = normalVector * speed;

    player->translate(kickbackVector * deltaTime);
}

void checkCollisions(const Scene& scene, const float deltaTime) {
    auto player = scene.player;
    AABB playerAABB = player->getCollider();

    for (auto& portal : scene.portals) {
        if (portal == nullptr || portal->getLinkedPortal() == nullptr) continue;

        if (playerAABB.intersects(portal->getAABB())) {
            portal->handleCollision(player);
            return;
        }
    }

    for (auto& collider : scene.colliders) {
        AABB colliderAABB = collider->getAABB();

        if (playerAABB.intersects(colliderAABB)) {
            handleCollision(player, collider, deltaTime);
        }
    }
}
