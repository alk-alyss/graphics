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

    if (!noClip) rotate(0, yaw, 0);

    camera->look(pitch, yaw);
}

void Player::moveForward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    // if looking up use inverse up vector as forward
    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        forwardVector = -up();
    }
    // if looking down use up vector as forward
    else if (dot(forward(), vec3(0,-1,0)) > 0.99) {
        forwardVector = up();
    }

    if (!noClip) {
        forwardVector.y = 0;
        translate(normalize(forwardVector) * movementSpeed * deltaTime);
        camera->setPosition(position);
    } else {
        camera->translate(normalize(forwardVector) * movementSpeed * deltaTime);
    }
}

void Player::moveBackward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    // if looking up use inverse up vector as forward
    if (dot(up(), vec3(0,1,0)) < 0.0001) {
        forwardVector = -up();
    }
    // if looking down use up vector as forward
    else if (dot(forward(), vec3(0,-1,0)) > 0.99) {
        forwardVector = up();
    }

    if (!noClip) {
        forwardVector.y = 0;
        translate(-normalize(forwardVector) * movementSpeed * deltaTime);
        camera->setPosition(position);
    } else {
        camera->translate(-normalize(forwardVector) * movementSpeed * deltaTime);
    }
}

void Player::moveLeft(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip) {
        rightVector.y = 0;
        translate(-normalize(rightVector) * movementSpeed * deltaTime);
        camera->setPosition(position);
    } else {
        camera->translate(-normalize(rightVector) * movementSpeed * deltaTime);
    }
}

void Player::moveRight(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip) {
        rightVector.y = 0;
        translate(normalize(rightVector) * movementSpeed * deltaTime);
        camera->setPosition(position);
    } else {
        camera->translate(normalize(rightVector) * movementSpeed * deltaTime);
    }
}

void Player::toggleNoClip() {
    if (noClip) {
        camera = std::make_unique<Camera>(position, forward(), up());
    }

    noClip = !noClip;
}
