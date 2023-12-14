#include "camera.hpp"

void FreeCamera::moveForward(float deltaTime) {
    translate(direction * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::moveBackward(float deltaTime) {
    translate(-direction * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::strafeLeft(float deltaTime) {
    translate(-right * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::strafeRight(float deltaTime) {
    translate(right * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FreeCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);

    updateViewMatrix();
}
