#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "model.hpp"
#include "orientable.hpp"
#include "camera.hpp"

class Player : public Orientable {
    std::unique_ptr<Camera> camera;

    bool noClip = false;

    float movementSpeed = 10.0f;
    float mouseSpeed = 0.1f;

public:
    Player(
        glm::vec3 position = DEFAULT_POSITION,
        glm::vec3 rotation = DEFAULT_ORIENTATION,
        glm::vec3 scale = DEFAULT_SCALE
    );

    void updateCamera(GLFWwindow* window) {camera->updateAspectRatio(window);}
    const Camera& getCamera() {return *camera;}

    void setMovementSpeed(float movementSpeed) {this->movementSpeed = movementSpeed;}
    void setMouseSpeed(float mouseSpeed) {this->mouseSpeed = mouseSpeed;}

    void look(float mouseX, float mouseY, float deltaTime);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveRight(float deltaTime);
    void moveLeft(float deltaTime);
    void zoom(float amount) {camera->zoom(amount);}

    void toggleNoClip();
};

#endif
