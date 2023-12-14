#include "camera.hpp"

void FirstPersonCamera::moveForward(float deltaTime) {
    glm::vec3 directionVector = direction;
    directionVector.y = 0;
    translate(directionVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveBackward(float deltaTime) {
    glm::vec3 directionVector = direction;
    directionVector.y = 0;
    translate(-directionVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::strafeLeft(float deltaTime) {
    glm::vec3 rightVector = right;
    rightVector.y = 0;
    translate(-rightVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::strafeRight(float deltaTime) {
    glm::vec3 rightVector = right;
    rightVector.y = 0;
    translate(rightVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);

    updateViewMatrix();
}
