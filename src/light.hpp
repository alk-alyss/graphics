#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <vector>

#include "common.hpp"

class Light : public Orientable {
protected:
    glm::vec4 La;
    glm::vec4 Ld;
    glm::vec4 Ls;
    float power;

public:
    Light(
        glm::vec4 La,
        glm::vec4 Ld,
        glm::vec4 Ls,
        float power,
        glm::vec3 position,
        glm::vec3 rotation
    ) : La(La), Ld(Ld), Ls(Ls), power(power), Orientable(position, rotation){}

    virtual std::vector<glm::vec4> data() const = 0;
};

class PointLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 4*sizeof(glm::vec4);
    }
};

class DirectionLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 4*sizeof(glm::vec4);
    }
};

class FlashLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 5*sizeof(glm::vec4);
    }
};

#endif
