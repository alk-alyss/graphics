#include "light.hpp"

std::vector<glm::vec4> PointLight::data() const {
    std::vector<glm::vec4> data = {
        glm::vec4(color, power),
        glm::vec4(position, 1)
    };

    return data;
}

std::vector<glm::vec4> DirectionalLight::data() const {
    std::vector<glm::vec4> data = {
        glm::vec4(color, power),
        glm::vec4(forward(), 0)
    };

    return data;
}

std::vector<glm::vec4> FlashLight::data() const {
    std::vector<glm::vec4> data = {
        glm::vec4(color, power),
        glm::vec4(position, 1),
        glm::vec4(forward(), 0)
    };

    return data;
}
