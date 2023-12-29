#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <vector>

#include "common.hpp"

class Light : public Orientable {
protected:
    glm::vec3 color;
    float power;

public:
    Light(
        glm::vec3 color,
        float power,
        glm::vec3 position,
        glm::vec3 rotation
    ) : color(color), power(power), Orientable(position, rotation){}

    virtual std::vector<glm::vec4> data() const = 0;
};

class PointLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 2*sizeof(glm::vec4);
    }
};

class DirectionalLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 2*sizeof(glm::vec4);
    }
};

class FlashLight : public Light {
public:
    using Light::Light;

    std::vector<glm::vec4> data() const;

    constexpr static size_t sizeofData() {
        return 3*sizeof(glm::vec4);
    }
};

#endif
