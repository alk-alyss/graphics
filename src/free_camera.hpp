#ifndef FREE_CAMERA_HPP
#define FREE_CAMERA_HPP

#include <camera.hpp>

class FreeCamera : public Camera {
private:
    float movementSpeed = 1.0f;
    float mouseSpeed = 1.0f;
public:
    FreeCamera() : Camera() {};
    FreeCamera(glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation) {};
    FreeCamera(glm::vec3 position, glm::vec3 rotation, float movementSpeed, float mouseSpeed): Camera(position, rotation) {
        this->movementSpeed = movementSpeed;
        this->mouseSpeed = mouseSpeed;
    }

    void setMovementSpeed(float movementSpeed) {this->movementSpeed = movementSpeed;}
    void setMouseSpeed(float mouseSpeed) {this->mouseSpeed = mouseSpeed;}

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void strafeLeft(float deltaTime);
    void strafeRight(float deltaTime);

    // mouseX and mouseY are relative to the center of the screen
    // TODO: find better name for method
    void look(float mouseX, float mouseY, float deltaTime);
};

#endif