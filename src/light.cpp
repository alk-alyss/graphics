#include "light.hpp"

std::vector<glm::vec4> Light::data() const {
    std::vector<glm::vec4> data;

    data.push_back(La);
    data.push_back(Ld);
    data.push_back(Ls);
    data.push_back(glm::vec4(position, 1));

    return data;
}
