#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "model.hpp"
#include "player.hpp"

class Portal: public Model {
private:
    std::shared_ptr<Portal> linkedPortal;
public:
    Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size);

    void linkPortal(std::shared_ptr<Portal> portal);
    void handleCollision(std::shared_ptr<Player> player);
};

#endif
