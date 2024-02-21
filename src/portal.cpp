#include "portal.hpp"
#include "resources.hpp"

Portal::Portal(const glm::vec3 position, glm::vec3 direction) : Model(planeMesh, metalMaterial){
    setPosition(position);
    lookAt(position+direction);
}
