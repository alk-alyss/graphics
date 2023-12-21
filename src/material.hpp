#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
    float Ns;
};

inline Material gold = {
    glm::vec4(0.628281, 0.555802, 0.366065, 1.0),
    glm::vec4(0.75164, 0.60648, 0.22648, 1.0),
    glm::vec4(0.24725, 0.1995, 0.0745, 1.0),
    51.2
};

inline Material silver = {
    glm::vec4(0.508273, 0.508273, 0.508273, 1),
    glm::vec4(0.50754, 0.50754, 0.50754, 1),
    glm::vec4(0.19225, 0.19225, 0.19225, 1),
    51.2
};

inline Material copper = {
    glm::vec4(0.256777, 0.137622, 0.086014, 1),
    glm::vec4(0.7038, 0.27048, 0.0828, 1),
    glm::vec4(0.19125, 0.0735, 0.0225, 1),
    12.8
};

inline Material emerald = {
    glm::vec4(0.633, 0.727811, 0.633, 1),
    glm::vec4(0.07568, 0.61424, 0.07568, 1),
    glm::vec4(0.0215, 0.1745, 0.0215, 1),
    76.8
};

#endif
