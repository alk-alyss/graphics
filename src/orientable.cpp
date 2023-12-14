#include "common.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::quat eulerRotation(glm::vec3 rotation) {
    glm::quat rotationX = glm::angleAxis(rotation.x, glm::vec3(1, 0, 0));
    glm::quat rotationY = glm::angleAxis(rotation.y, glm::vec3(0, 1, 0));
    glm::quat rotationZ = glm::angleAxis(rotation.z, glm::vec3(0, 0, 1));

    return rotationX * rotationY * rotationZ;
}

glm::vec3 Orientable::up() {
    return glm::cross(right, direction);
}

void Orientable::translate(glm::vec3 translation) {
    position += translation;
}

void Orientable::rotate(glm::vec3 rotation) {
    glm::quat rotationQuat = eulerRotation(rotation);

    direction = rotationQuat * direction;
    right = rotationQuat * right;
}

void Orientable::rotate(float angle, glm::vec3 axis) {
    glm::quat rotationQuat = glm::angleAxis(angle, axis);

    direction = rotationQuat * direction;
    right = rotationQuat * right;
}

void Orientable::setRotation(glm::vec3 rotation) {
    glm::quat rotationQuat = eulerRotation(rotation);

    direction = rotationQuat * glm::vec3(0, 0, -1);
    right = rotationQuat * glm::vec3(1, 0, 0);
}

glm::vec3 Orientable::getRotation() {
    // TODO: implement
    return glm::vec3(0,0,0);
}

void lookAt(glm::vec3 target, glm::vec3 up) {
    // TODO: implement
}
