#include "orientable.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

quat eulerToQuat(vec3 rotation) {
    quat rotationX = angleAxis(rotation.x, vec3(1,0,0));
    quat rotationY = angleAxis(rotation.y, vec3(0,1,0));
    quat rotationZ = angleAxis(rotation.z, vec3(0,0,1));

    return rotationZ * rotationY * rotationX;
}

Orientable::Orientable(vec3 position, vec3 rotation, vec3 scale) {
    this->position = position;
    setRotation(rotation);
    this->scale = scale;
}

vec3 Orientable::up() const {
    return orientation * vec3(0,1,0);
}

vec3 Orientable::forward() const {
    return orientation * vec3(0,0,-1);
}

vec3 Orientable::right() const {
    return orientation * vec3(1,0,0);
}

void Orientable::translate(vec3 translation) {
    position += translation;
}

void Orientable::rotate(vec3 rotation) {
    quat rotationQuat = normalize(eulerToQuat(rotation));

    orientation = normalize(rotationQuat * orientation);
}

void Orientable::rotate(float angle, vec3 axis) {
    quat rotationQuat = normalize(angleAxis(angle, axis));

    orientation = normalize(rotationQuat * orientation);
}

void Orientable::setRotation(vec3 rotation) {
    orientation = normalize(eulerToQuat(rotation));
}

void Orientable::changeScale(vec3 scaleFactor) {
    scale += scaleFactor;
}

mat4 Orientable::translationMatrix() const {
    return glm::translate(glm::mat4(1), position);
}

mat4 Orientable::rotationMatrix() const {
    return toMat4(orientation);
}

mat4 Orientable::scallingMatrix() const {
    return glm::scale(glm::mat4(1), scale);
}

mat4 Orientable::modelMatrix() const {
    return translationMatrix() * rotationMatrix() * scallingMatrix();
}

void Orientable::lookAt(const vec3 target, const vec3 up, const vec3 alternativeUp) {
    assert(target != position);

    vec3 direction = target - position;

    if(length(direction) < 0.0001) {
        orientation = glm::quat(1, 0, 0, 0);
        return;
    }

    direction = normalize(direction);


    // Is the normal up (nearly) parallel to direction?
    if(glm::abs(glm::dot(direction, up)) > .9999f) {
        orientation = normalize(glm::quatLookAt(direction, alternativeUp));
    }
    else {
        orientation = normalize(glm::quatLookAt(direction, up));
    }
}
