#include "player.hpp"

using namespace glm;

Player::Player(
    std::shared_ptr<Model> model,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
) : Orientable(position, rotation, scale), model(model) {
    camera = std::make_unique<Camera>(position, rotation);
    camera->setNC(1.0);

    model->setPosition(position);
    model->setRotation(rotation);
    model->setScale(scale);
};

void Player::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = -mouseY * mouseSpeed * deltaTime;
    float yaw = -mouseX * mouseSpeed * deltaTime;

    camera->look(pitch, yaw);
    lookAt(camera->forward() + position, camera->up());

    if(!noClip) model->rotate(0,yaw,0);
}

void Player::moveForward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    if (!noClip) {
        // if forward vector is vertical
        if (dot(up(), vec3(0,1,0)) < 0.0001) {
            // use inverse up vector if looking up
            forwardVector = -up();
            // use up vector if looking down
            if (dot(forward(), vec3(0,-1,0)) > 0.99) {
                forwardVector = up();
            }
        }

        forwardVector.y = 0;

        model->setPosition(position);
    }

    translate(normalize(forwardVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveBackward(float deltaTime) {
    glm::vec3 forwardVector = forward();

    if (!noClip) {
        // if forward vector is vertical
        if (dot(up(), vec3(0,1,0)) < 0.0001) {
            // use inverse up vector if looking up
            forwardVector = -up();
            // use up vector if looking down
            if (dot(forward(), vec3(0,-1,0)) > 0.99) {
                forwardVector = up();
            }
        }

        forwardVector.y = 0;

        model->setPosition(position);
    }

    translate(-normalize(forwardVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveLeft(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip) {
        rightVector.y = 0;

        model->setPosition(position);
    }

    translate(-normalize(rightVector) * movementSpeed * deltaTime);
    camera->setPosition(position);
}

void Player::moveRight(float deltaTime) {
    glm::vec3 rightVector = right();

    if (!noClip) {
        rightVector.y = 0;

        model->setPosition(position);
    }

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

void Player::draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const {
    model->draw(modelMatrix, shader);
}
