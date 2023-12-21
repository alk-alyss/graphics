#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
    float Ns;
};

#endif
