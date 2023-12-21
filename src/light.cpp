#include "light.hpp"

std::vector<glm::vec4> PointLight::data() const {
    std::vector<glm::vec4> data = {
        La,
        Ld,
        Ls,
        glm::vec4(position, 1)
    };

    return data;
}

std::vector<glm::vec4> DirectionLight::data() const {
    std::vector<glm::vec4> data = {
        La,
        Ld,
        Ls,
        glm::vec4(forward(), 0)
    };

    return data;
}

std::vector<glm::vec4> FlashLight::data() const {
    std::vector<glm::vec4> data = {
        La,
        Ld,
        Ls,
        glm::vec4(position, 1),
        glm::vec4(forward(), 0)
    };

    return data;
}
