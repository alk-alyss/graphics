#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "model.hpp"

class Portal: public Model {
public:
    Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size);
};

#endif
