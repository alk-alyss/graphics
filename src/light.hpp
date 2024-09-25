#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <vector>

#include "orientable.hpp"

class Light : public Orientable {
protected:
    glm::vec3 color = glm::vec3(1,1,1);
    float power = 10;

public:
    Light() : Light(DEFAULT_POSITION, DEFAULT_ORIENTATION, 10, glm::vec3(1,1,1)) {}
    Light(glm::vec3 position) : Light(position, DEFAULT_ORIENTATION, 10, glm::vec3(1,1,1)) {}
    Light(glm::vec3 position, glm::quat rotation) : Light(position, rotation, 10, glm::vec3(1,1,1)) {}
    Light(glm::vec3 position, glm::quat rotation, float power) : Light(position, rotation, power, glm::vec3(1,1,1)) {}
    Light(
        glm::vec3 position,
        glm::quat rotation,
        float power,
        glm::vec3 color
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
