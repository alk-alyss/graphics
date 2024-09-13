#include "portal.hpp"
#include "resources.hpp"

Portal::Portal(
    const glm::vec3 position,
    const glm::vec3 direction,
    const glm::vec3 size
) : Model(planeMesh, metalMaterial, Transformation(glm::vec3(0), glm::vec3(glm::radians(-90.0f), 0, 0))) {
    setPosition(position);
    setScale(size);
    lookAt(position+direction);
}

void Portal::linkPortal(std::shared_ptr<Portal> portal) {
    linkedPortal = portal;
}

#include <iostream>
#include <glm/gtx/string_cast.hpp>
void Portal::handleCollision(std::shared_ptr<Player> player) {
    if (linkedPortal == nullptr) return;

    glm::vec3 portalNormal = linkedPortal->forward();
    glm::vec3 newPosition = linkedPortal->getPosition() + portalNormal;

    glm::vec3 playerForward = player->forward();
    glm::vec3 lookAt = newPosition + portalNormal;;

    std::cout << glm::to_string(portalNormal) << " " << glm::to_string(newPosition) << " " << glm::to_string(lookAt) << std::endl;

    player->translate(newPosition - player->getPosition());
    player->lookAt(lookAt);
}
