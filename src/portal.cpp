#include "portal.hpp"
#include "resources.hpp"

Portal::Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size)
    : Model(planeMesh, metalMaterial, Transformation(glm::vec3(0), glm::vec3(glm::radians(-90.0f), 0, 0))){
    setPosition(position);
    setScale(size);
    lookAt(position+direction);
}
