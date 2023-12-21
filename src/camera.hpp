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

    float movementSpeed = 10.0f;
    float mouseSpeed = 0.1f;

protected:
    void updateViewMatrix();
    void updateProjectionMatrix();

public:
    Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0f)) {};
    Camera(glm::vec3 position, glm::vec3 rotation);
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    glm::mat4 getView() const {return viewMatrix;}
    glm::mat4 getProjection() const {return projectionMatrix;}
    glm::mat4 getVP() const {return VP;}

    void updateAspectRatio(GLFWwindow* window);

    void zoom(float amount);

    void setMovementSpeed(float movementSpeed) {this->movementSpeed = movementSpeed;}
    void setMouseSpeed(float mouseSpeed) {this->mouseSpeed = mouseSpeed;}

    virtual void moveForward(float deltaTime) = 0;
    virtual void moveBackward(float deltaTime) = 0;
    virtual void moveLeft(float deltaTime) = 0;
    virtual void moveRight(float deltaTime) = 0;

    void look(float mouseX, float mouseY, float deltaTime);
};

class FirstPersonCamera : public Camera {
public:
    FirstPersonCamera() : Camera() {};
    FirstPersonCamera(glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation) {};
    FirstPersonCamera(glm::vec3 position, glm::vec3 rotation, float movementSpeed, float mouseSpeed): Camera(position, rotation) {
        this->movementSpeed = movementSpeed;
        this->mouseSpeed = mouseSpeed;
    }

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
};

class FreeCamera : public Camera {
public:
    FreeCamera() : Camera() {};
    FreeCamera(glm::vec3 position, glm::vec3 rotation) : Camera(position, rotation) {};
    FreeCamera(glm::vec3 position, glm::vec3 rotation, float movementSpeed, float mouseSpeed): Camera(position, rotation) {
        this->movementSpeed = movementSpeed;
        this->mouseSpeed = mouseSpeed;
    }

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
};
#endif
