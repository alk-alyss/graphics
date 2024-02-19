#include "collisions.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void checkCollisions(const Scene& scene) {
    AABB playerAABB = scene.player->getCollider();

    for (auto& model : scene.models) {
        AABB modelAABB = model->getAABB();

        if (playerAABB.intersects(modelAABB)) {
            std::cout << "collides" << model << std::endl;
        }
    }
}
