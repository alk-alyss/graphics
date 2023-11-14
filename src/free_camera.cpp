#include <free_camera.hpp>

void FreeCamera::moveForward(float deltaTime) {
    glm::vec3 direction = getDirectionVector();
    translate(direction * movementSpeed * deltaTime);
}

void FreeCamera::moveBackward(float deltaTime) {
    glm::vec3 direction = getDirectionVector();
    translate(-direction * movementSpeed * deltaTime);
}

void FreeCamera::strafeLeft(float deltaTime) {
    glm::vec3 right = getRightVector();
    translate(-right * movementSpeed * deltaTime);
}

void FreeCamera::strafeRight(float deltaTime) {
    glm::vec3 right = getRightVector();
    translate(right * movementSpeed * deltaTime);
}

void FreeCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);
}