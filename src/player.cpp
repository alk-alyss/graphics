#include "player.hpp"

using namespace glm;

Player::Player(
    std::shared_ptr<Model> model,
    glm::vec3 position,
    glm::quat orientation,
    glm::vec3 scale
) : Orientable(position, orientation, scale), model(model) {
    camera = std::make_unique<Camera>(position, orientation);
    camera->setNC(0.1);

    model->setPosition(position);
    model->setOrientation(orientation);
    model->setScale(scale);
};

void Player::update(float deltaTime) {
    translate(velocity * deltaTime);
    velocity = glm::vec3(0);
}

void Player::updateComponents() {
    camera->setPosition(position);
    camera->setOrientation(orientation);

    if (noClip) return;
    model->setPosition(position);
    model->setOrientation(orientation);
    model->setScale(scale);
}

void Player::look(float mouseX, float mouseY, float deltaTime) {
    float pitch = -mouseY * mouseSpeed * deltaTime;
    float yaw = -mouseX * mouseSpeed * deltaTime;

    camera->look(pitch, yaw);
    lookAt(camera->forward() + position, camera->up());
}

void Player::lookAt(glm::vec3 target, glm::vec3 up, glm::vec3 alternativeUp) {
    Orientable::lookAt(target, up, alternativeUp);
    updateComponents();
}

void Player::setPosition(glm::vec3 position) {
    previousPosition = position;
    Orientable::setPosition(position);
    updateComponents();
}

void Player::setOrientation(glm::quat rotation) {
    Orientable::setOrientation(rotation);
    updateComponents();
}

void Player::translate(glm::vec3 translation) {
    previousPosition = position;
    Orientable::translate(translation);
    updateComponents();
}

void Player::rotate(glm::quat rotation) {
    Orientable::rotate(rotation);
    updateComponents();
}

glm::vec3 Player::forwardMovementVector() {
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
    }

    return forwardVector;
}

void Player::moveForward() {
    glm::vec3 forwardVector = forwardMovementVector();

    if (glm::length(velocity) > 0) velocity = normalize(velocity);

    velocity += forwardVector;
    velocity = normalize(velocity) * movementSpeed;
}

void Player::moveBackward() {
    glm::vec3 forwardVector = forwardMovementVector();

    if (glm::length(velocity) > 0) velocity = normalize(velocity);

    velocity -= forwardVector;
    velocity = normalize(velocity) * movementSpeed;
}

void Player::moveLeft() {
    glm::vec3 rightVector = right();
    if (!noClip) rightVector.y = 0;

    if (glm::length(velocity) > 0) velocity = normalize(velocity);

    velocity -= rightVector;
    velocity = normalize(velocity) * movementSpeed;
}

void Player::moveRight() {
    glm::vec3 rightVector = right();
    if (!noClip) rightVector.y = 0;

    if (glm::length(velocity) > 0) velocity = normalize(velocity);

    velocity += rightVector;
    velocity = normalize(velocity) * movementSpeed;
}

void Player::toggleNoClip() {
    if (noClip) {
        position = model->getPosition();
        orientation = model->getOrientation();

        camera->setPosition(position);
        camera->lookAt(forward() + position, up());
    }

    noClip = !noClip;
}

void Player::draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const {
    model->draw(modelMatrix, shader);
}
