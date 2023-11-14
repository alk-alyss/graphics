#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
protected:
    float nearCP;
    float farCP;

    float fov;
    float aspectRatio;

    glm::vec3 position;
    glm::vec3 rotation; // pitch, yaw, roll in degrees

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::mat4 VP;

protected:
    void updateViewMatrix();
    void updateProjectionMatrix();

    glm::vec3 getDirectionVector();
    glm::vec3 getUpVector();
    glm::vec3 getRightVector();

public:
    Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0f)) {};
    Camera(glm::vec3 position, glm::vec3 rotation);
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up) : Camera(position, calculateRotation(target, up)) {};

    glm::mat4 getVP() {return VP;}

    void setPosition(float x, float y, float z) {this->position = glm::vec3(x, y, z);}
    void setPosition(glm::vec3 position) {this->position = position;}

    void setRotation(float pitch, float yaw, float roll) {this->rotation = glm::vec3(pitch, yaw, roll);}
    void setRotation(glm::vec3 rotation) {this->rotation = rotation;}

    glm::vec3 getPosition() {return this->position;}
    glm::vec3 getRotation() {return this->rotation;}

    glm::vec3 calculateRotation(glm::vec3 direction, glm::vec3 up); 

    void updateAspectRatio(GLFWwindow* window);

    void translate(float x, float y, float z) {translate(glm::vec3(x, y, z));}
    void translate(glm::vec3 translation);

    void rotate(float pitch, float yaw, float roll) {rotate(glm::vec3(pitch, yaw, roll));}
    void rotate(float pitch, float yaw) {rotate(pitch, yaw, 0.0f);}
    void rotate(glm::vec3 rotation);
    void rotate(float angle, glm::vec3 axis);

    void zoom(float amount);

    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
};

#endif