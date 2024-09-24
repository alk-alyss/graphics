#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "orientable.hpp"

class Camera : public Orientable {
protected:
    float nearCP = 0.1f;
    float farCP = 500.0f;

    float fov = 65.0f;
    float aspectRatio = 16.0f/9.0f;

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    glm::mat4 VP = glm::mat4(1.0f);

protected:
    void updateViewMatrix();
    void updateProjectionMatrix();

public:
    Camera() : Camera(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)) {};
    Camera(glm::vec3 position, glm::quat orientation);
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    glm::mat4 getView() const {return viewMatrix;}
    glm::mat4 getProjection() const {return projectionMatrix;}
    glm::mat4 getVP() const {return VP;}

    void setNC(float value) {nearCP = value;}
    void setFC(float value) {farCP = value;}

    void updateAspectRatio(GLFWwindow* window);

    void zoom(float amount);

    void setProjection(glm::mat4 newProjectionMatrix) {projectionMatrix = newProjectionMatrix;}

    void setPosition(glm::vec3 position);
    void setOrientation(glm::quat orientation);
    void setScale(glm::vec3 newScale);

    void translate(glm::vec3 translation);

    void changeScale(glm::vec3 scaleFactor);

    void rotate(glm::quat rotation);
    void rotate(float angle, glm::vec3 axis);

    void look(float pitch, float yaw);
    void lookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
