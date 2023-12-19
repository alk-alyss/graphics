#include "camera.hpp"

void FirstPersonCamera::moveForward(float deltaTime) {
    glm::vec3 directionVector = direction();
    directionVector.y = 0;
    translate(directionVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveBackward(float deltaTime) {
    glm::vec3 directionVector = direction();
    directionVector.y = 0;
    translate(-directionVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveLeft(float deltaTime) {
    glm::vec3 rightVector = right();
    rightVector.y = 0;
    translate(-rightVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveRight(float deltaTime) {
    glm::vec3 rightVector = right();
    rightVector.y = 0;
    translate(rightVector * movementSpeed * deltaTime);

    updateViewMatrix();
}
