#include "camera.hpp"

void FreeCamera::moveForward(float deltaTime) {
    translate(direction() * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::moveBackward(float deltaTime) {
    translate(-direction() * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::moveLeft(float deltaTime) {
    translate(-right() * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::moveRight(float deltaTime) {
    translate(right() * movementSpeed * deltaTime);

    updateViewMatrix();
}
