#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "orientable.hpp"

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

    glm::mat4 getView() const {return viewMatrix;}
    glm::mat4 getProjection() const {return projectionMatrix;}
    glm::mat4 getVP() const {return VP;}

    void setNC(float value) {nearCP = value;}
    void setFC(float value) {farCP = value;}

    void updateAspectRatio(GLFWwindow* window);

    void zoom(float amount);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 newScale);

    void translate(glm::vec3 translation);

    void changeScale(glm::vec3 scaleFactor);

    void rotate(glm::vec3 rotation);
    void rotate(float angle, glm::vec3 axis);

    void look(float pitch, float yaw);
    void lookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
