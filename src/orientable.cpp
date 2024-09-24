#include "orientable.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

quat eulerToQuat(float pitch, float yaw, float roll) {
    quat rotationX = angleAxis(pitch, vec3(1,0,0));
    quat rotationY = angleAxis(yaw, vec3(0,1,0));
    quat rotationZ = angleAxis(roll, vec3(0,0,1));

    return normalize(rotationZ * rotationY * rotationX);
}

Orientable::Orientable(vec3 position, quat orientation, vec3 scale) {
    this->position = position;
    setOrientation(orientation);
    this->scale = scale;
}

// Cartesian vectors
vec3 Orientable::up() const {
    return orientation * vec3(0,1,0);
}
vec3 Orientable::forward() const {
    return orientation * vec3(0,0,-1);
}
vec3 Orientable::right() const {
    return orientation * vec3(1,0,0);
}

// Setters
void Orientable::setPosition(glm::vec3 position) {
    this->position = position;
}
void Orientable::setOrientation(quat orientation) {
    this->orientation = normalize(orientation);
}
void Orientable::setScale(vec3 scale) {
    this->scale = scale;
}

// Transformations
void Orientable::translate(vec3 translation) {
    position += translation;
}
void Orientable::rotate(quat rotation) {
    quat rotationQuat = normalize(rotation);

    orientation = normalize(rotationQuat * orientation);
}
void Orientable::rotate(float angle, vec3 axis) {
    quat rotationQuat = normalize(angleAxis(angle, axis));

    orientation = normalize(rotationQuat * orientation);
}
void Orientable::changeScale(vec3 scaleFactor) {
    scale += scaleFactor;
}

// Matrices
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

// Look at target
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
