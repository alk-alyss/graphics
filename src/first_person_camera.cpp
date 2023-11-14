#include <first_person_camera.hpp>

void FirstPersonCamera::moveForward(float deltaTime) {
    glm::vec3 directionVector = direction(rotation);
    directionVector.y = 0;
    translate(directionVector * movementSpeed * deltaTime);
}

void FirstPersonCamera::moveBackward(float deltaTime) {
    glm::vec3 directionVector = direction(rotation);
    directionVector.y = 0;
    translate(-directionVector * movementSpeed * deltaTime);
}

void FirstPersonCamera::strafeLeft(float deltaTime) {
    glm::vec3 rightVector = right(rotation);
    rightVector.y = 0;
    translate(-rightVector * movementSpeed * deltaTime);
}

void FirstPersonCamera::strafeRight(float deltaTime) {
    glm::vec3 rightVector = right(rotation);
    rightVector.y = 0;
    translate(rightVector * movementSpeed * deltaTime);
}

void FirstPersonCamera::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = mouseX * glm::radians(fov) * aspectRatio * mouseSpeed * deltaTime;
    float yaw = mouseY * glm::radians(fov) * mouseSpeed * deltaTime;

    rotate(pitch, yaw);
}