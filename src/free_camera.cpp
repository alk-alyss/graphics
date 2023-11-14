#include <free_camera.hpp>

void FreeCamera::moveForward(float deltaTime) {
    translate(direction(rotation) * movementSpeed * deltaTime);
}

void FreeCamera::moveBackward(float deltaTime) {
    translate(-direction(rotation) * movementSpeed * deltaTime);
}

void FreeCamera::strafeLeft(float deltaTime) {
    translate(-right(rotation) * movementSpeed * deltaTime);
}

void FreeCamera::strafeRight(float deltaTime) {
    translate(right(rotation) * movementSpeed * deltaTime);
}

void FreeCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);
}