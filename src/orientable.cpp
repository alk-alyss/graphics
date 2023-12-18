#include "common.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

vec3 Orientable::up() {
    return orientation * vec3(0,1,0);
}

vec3 Orientable::forward() {
    return orientation * vec3(0,0,-1);
}

vec3 Orientable::right() {
    return orientation * vec3(1,0,0);
}

void Orientable::translate(vec3 translation) {
    position += translation;
}

void Orientable::rotate(vec3 rotation) {
    quat rotationQuat = quat(rotation);

    orientation = rotationQuat * orientation;
}

void Orientable::rotate(float angle, vec3 axis) {
    quat rotationQuat = angleAxis(angle, axis);

    orientation = rotationQuat * orientation;
}

void Orientable::setRotation(vec3 rotation) {
    quat rotationQuat = quat(rotation);

    orientation = rotationQuat * quat(vec3(0,0,0));
}

mat4 Orientable::getRotationMatrix() {
    return toMat4(orientation);
}

void Orientable::lookAt(vec3 target, vec3 up, vec3 alternativeUp) {
    assert(target != position);

    vec3 direction = target - position;

    if(length(direction) < 0.0001) {
        orientation = glm::quat(1, 0, 0, 0);
        return;
    }

    direction = normalize(direction);


    // Is the normal up (nearly) parallel to direction?
    if(glm::abs(glm::dot(direction, up)) > .9999f) {
        orientation = glm::quatLookAt(direction, alternativeUp);
    }
    else {
        orientation = glm::quatLookAt(direction, up);
    }
}
