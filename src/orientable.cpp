#include "common.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

vec3 Orientable::up() {
    return cross(right, direction);
}

void Orientable::translate(vec3 translation) {
    position += translation;
}

void Orientable::rotate(vec3 rotation) {
    quat rotationQuat = quat(rotation);

    direction = rotationQuat * direction;
    right = rotationQuat * right;
}

void Orientable::rotate(float angle, vec3 axis) {
    quat rotationQuat = angleAxis(angle, axis);

    direction = rotationQuat * direction;
    right = rotationQuat * right;
}

void Orientable::setRotation(vec3 rotation) {
    quat rotationQuat = quat(rotation);

    direction = rotationQuat * vec3(0, 0, -1);
    right = rotationQuat * vec3(1, 0, 0);
}

mat4 Orientable::getRotationMatrix() {
    return glm::lookAt(vec3(0), direction, up());
}

void Orientable::lookAt(vec3 target, vec3 up) {
    direction = normalize(target);
    right = cross(direction, up);
}
