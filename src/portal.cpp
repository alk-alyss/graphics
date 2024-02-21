#include "portal.hpp"
#include "resources.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>
Portal::Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size)
    : Model(planeMesh, metalMaterial, Transformation(glm::vec3(0), glm::vec3(glm::radians(-90.0f), 0, 0))){
    setPosition(position);
    setScale(size);
    lookAt(position+direction);
    std::cout << glm::to_string(position) << " " << glm::to_string(direction) << std::endl;
    std::cout << glm::to_string(position+direction) << std::endl;
}
