#include <first_person_camera.hpp>

void FirstPersonCamera::moveForward(float deltaTime) {
    glm::vec3 direction = getDirectionVector();
    direction.y = 0;
    translate(direction * movementSpeed * deltaTime);
}

void FirstPersonCamera::moveBackward(float deltaTime) {
    glm::vec3 direction = getDirectionVector();
    direction.y = 0;
    translate(-direction * movementSpeed * deltaTime);
}

void FirstPersonCamera::strafeLeft(float deltaTime) {
    glm::vec3 right = getRightVector();
    right.y = 0;
    translate(-right * movementSpeed * deltaTime);
}

void FirstPersonCamera::strafeRight(float deltaTime) {
    glm::vec3 right = getRightVector();
    right.y = 0;
    translate(right * movementSpeed * deltaTime);
}

void FirstPersonCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);
}