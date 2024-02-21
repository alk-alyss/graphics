#include "collisions.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void handleCollision(const std::shared_ptr<Player> player, const std::shared_ptr<Model> model, float deltaTime) {

    /* std::cout << "collides" << model << std::endl; */

    glm::vec3 playerPosition = player->getPosition();
    glm::vec3 modelPosition = model->getPosition();

    glm::vec3 modelToPlayer = glm::normalize(playerPosition - modelPosition);
    glm::vec3 normalVector = modelToPlayer;

    float xDot = glm::dot(modelToPlayer, glm::vec3(1, 0, 0));
    float zDot = glm::dot(modelToPlayer, glm::vec3(0, 0, 1));
    /* std::cout << "xDot: " << xDot << " zDot: " << zDot << " "; */

    float limit = 0.5;
    if (std::abs(xDot) > std::abs(zDot)) {
        if (xDot > limit) {
            normalVector = glm::vec3(1,0,0);
        } else if (xDot < -limit) {
            normalVector = glm::vec3(-1,0,0);
        }
    } else {
        if (zDot > limit) {
            normalVector = glm::vec3(0,0,1);
        } else if (zDot < -limit) {
            normalVector = glm::vec3(0,0,-1);
        }
    }

    glm::vec3 playerVelocity = player->getVelocity();
    glm::vec3 kickbackVector = glm::reflect(playerVelocity, normalVector);

    float speed = glm::dot(normalVector, kickbackVector);
    if (speed < 0) speed = 0;

    kickbackVector = normalVector * speed;
    std::cout << glm::to_string(normalVector) << " " << glm::to_string(kickbackVector) << std::endl;

    player->translate(kickbackVector * deltaTime);
}

void checkCollisions(const Scene& scene, float deltaTime) {
    auto player = scene.player;
    AABB playerAABB = player->getCollider();

    for (auto& model : scene.models) {
        AABB modelAABB = model->getAABB();

        if (playerAABB.intersects(modelAABB)) {
            handleCollision(player, model, deltaTime);
        }
    }
}
