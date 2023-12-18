#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "common.hpp"

class Camera : public Orientable {
protected:
    float nearCP;
    float farCP;

    float fov;
    float aspectRatio;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::mat4 VP;

protected:
    void updateViewMatrix();
    void updateProjectionMatrix();

public:
    Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0f)) {};
    Camera(glm::vec3 position, glm::vec3 rotation);
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    glm::mat4 getVP() {return VP;}

    void updateAspectRatio(GLFWwindow* window);

    void zoom(float amount);
};

class FirstPersonCamera : public Camera {
private:
    float movementSpeed = 10.0f;
    float mouseSpeed = 1.0f;
public:
    FirstPersonCamera() : Camera() {};
    FirstPersonCamera(glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation) {};
    FirstPersonCamera(glm::vec3 position, glm::vec3 rotation, float movementSpeed, float mouseSpeed): Camera(position, rotation) {
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
