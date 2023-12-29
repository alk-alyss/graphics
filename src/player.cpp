#include "player.hpp"

using namespace glm;

Player::Player(
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
) : Orientable(position, rotation, scale) {
    camera = std::make_unique<Camera>(position, rotation);
};

void Player::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = -mouseY * mouseSpeed * deltaTime;
    float yaw = -mouseX * mouseSpeed * deltaTime;

    camera->look(pitch, yaw);
    lookAt(camera->forward() + position, camera->up());
}

void Player::moveForward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    // if forward vector is vertical
    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        // use inverse up vector if looking up
        forwardVector = -up();
        // use up vector if looking down
        if (dot(forward(), vec3(0,-1,0)) > 0.99) {
            forwardVector = up();
        }
    }

    if (!noClip) forwardVector.y = 0;

    translate(normalize(forwardVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveBackward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    // if forward vector is vertical
    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        // use inverse up vector if looking up
        forwardVector = -up();
        // use up vector if looking down
        if (dot(forward(), vec3(0,-1,0)) > 0.99) {
            forwardVector = up();
        }
    }

    if (!noClip) forwardVector.y = 0;

    translate(-normalize(forwardVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveLeft(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip)  rightVector.y = 0;

    translate(-normalize(rightVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveRight(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip)  rightVector.y = 0;

    translate(normalize(rightVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::toggleNoClip() {
    if (noClip) {
        position = originalPosition;
        orientation = originalOrientation;

        camera->setPosition(position);
        camera->lookAt(forward() + position, up());
    } else {
        originalPosition = position;
        originalOrientation = orientation;
    }

    noClip = !noClip;
}
