#include "entity.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

quat eulerToQuat(const float pitch, const float yaw, const float roll) {
    quat rotationX = angleAxis(pitch, vec3(1,0,0));
    quat rotationY = angleAxis(yaw, vec3(0,1,0));
    quat rotationZ = angleAxis(roll, vec3(0,0,1));

    return normalize(rotationZ * rotationY * rotationX);
}

Entity::Entity(const vec3 position, const quat orientation, const vec3 scale) {
    this->position = position;
    setOrientation(orientation);
    this->scale = scale;
}

// Cartesian vectors
vec3 Entity::up() const {
    return orientation * vec3(0,1,0);
}
vec3 Entity::forward() const {
    return orientation * vec3(0,0,-1);
}
vec3 Entity::right() const {
    return orientation * vec3(1,0,0);
}

// Setters
void Entity::setPosition(const glm::vec3 position) {
    this->position = position;
}
void Entity::setOrientation(const quat orientation) {
    this->orientation = normalize(orientation);
}
void Entity::setScale(const vec3 scale) {
    this->scale = scale;
}

// Transformations
void Entity::translate(const vec3 translation) {
    position += translation;
}
void Entity::rotate(const quat rotation) {
    quat rotationQuat = normalize(rotation);

    orientation = normalize(rotationQuat * orientation);
}
void Entity::rotate(const float angle, const vec3 axis) {
    quat rotationQuat = normalize(angleAxis(angle, axis));

    orientation = normalize(rotationQuat * orientation);
}
void Entity::changeScale(const vec3 scaleFactor) {
    scale += scaleFactor;
}

// Matrices
mat4 Entity::translationMatrix() const {
    return glm::translate(glm::mat4(1), position);
}
mat4 Entity::rotationMatrix() const {
    return toMat4(orientation);
}
mat4 Entity::scallingMatrix() const {
    return glm::scale(glm::mat4(1), scale);
}
mat4 Entity::modelMatrix() const {
    return translationMatrix() * rotationMatrix() * scallingMatrix();
}

// Look at target
void Entity::lookAt(const vec3 target, const vec3 up, const vec3 alternativeUp) {
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
