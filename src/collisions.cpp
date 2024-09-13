#include "collisions.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void handleCollision(const std::shared_ptr<Player> player, const std::shared_ptr<Model> model, const float deltaTime) {
    glm::vec3 normalVector = model->getClosestBlockNormal(player->getPosition());

    glm::vec3 playerVelocity = player->getVelocity();
    glm::vec3 kickbackVector = glm::reflect(playerVelocity, normalVector);

    float speed = glm::dot(normalVector, kickbackVector);
    if (speed < 0) speed = 0;

    kickbackVector = normalVector * speed;
    /* std::cout << glm::to_string(normalVector) << " " << glm::to_string(kickbackVector) << std::endl; */

    player->translate(kickbackVector * deltaTime);
}

void checkCollisions(const Scene& scene, const float deltaTime) {
    auto player = scene.player;
    AABB playerAABB = player->getCollider();

    for (auto& model : scene.models) {
        AABB modelAABB = model->getAABB();

        if (playerAABB.intersects(modelAABB)) {
            handleCollision(player, model, deltaTime);
        }
    }

    auto portal1 = scene.portals.first;
    auto portal2 = scene.portals.second;

    if (portal1 != nullptr && playerAABB.intersects(portal1->getAABB())) {
        std::cout << "portal1 collision" << std::endl;
        portal1->handleCollision(player);
    } else if (portal2 != nullptr && playerAABB.intersects(portal2->getAABB())) {
        std::cout << "portal2 collision" << std::endl;
        portal2->handleCollision(player);
    }
}

std::shared_ptr<Model> castRay(
    const Scene& scene,
    const glm::vec3 position,
    const glm::vec3 direction,
    glm::vec3& normalVector
) {
    glm::vec3 rayPosition = position;
    float stepSize = 0.2;
    glm::vec3 rayDirection = glm::normalize(direction) * stepSize;

    int stepLimit = 200;
    for (int i=0; i<stepLimit; i++) {
        for (auto& model : scene.models) {
            AABB modelAABB = model->getAABB();

            if (modelAABB.intersects(rayPosition)) {
                /* std::cout << "collides" << model << std::endl; */
                normalVector = model->getClosestBlockNormal(rayPosition);

                return model;
            }
        }

        rayPosition += rayDirection;
    }

    return nullptr;
}
