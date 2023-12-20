#include "camera.hpp"

using namespace glm;

void FirstPersonCamera::moveForward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        forwardVector = -up();
        if (dot(forward(), vec3(0,-1,0)) > 0.99) {
            forwardVector = up();
        }
    }

    forwardVector.y = 0;
    forwardVector = normalize(forwardVector);

    translate(forwardVector * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveBackward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        forwardVector = -up();
        if (dot(forward(), vec3(0,-1,0)) > 0.99) {
            forwardVector = up();
        }
    }

    forwardVector.y = 0;
    translate(-normalize(forwardVector) * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveLeft(float deltaTime) {
    glm::vec3 rightVector = right();
    rightVector.y = 0;
    translate(-normalize(rightVector) * movementSpeed * deltaTime);

    updateViewMatrix();
}

void FirstPersonCamera::moveRight(float deltaTime) {
    glm::vec3 rightVector = right();
    rightVector.y = 0;
    translate(normalize(rightVector) * movementSpeed * deltaTime);

    updateViewMatrix();
}
