#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Camera::Camera(vec3 position, quat orientation) : Orientable(position, orientation){
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::Camera(vec3 position, vec3 target, vec3 up) {
    setPosition(position);
    lookAt(target, up);
    updateViewMatrix();
    updateProjectionMatrix();
};

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + forward(), up());

    VP = projectionMatrix * viewMatrix;
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = perspective(radians(fov), aspectRatio, nearCP, farCP);

    VP = projectionMatrix * viewMatrix;
}

void Camera::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = (float) width / (float) height;

    updateProjectionMatrix();
}

void Camera::zoom(float amount) {
    fov += amount;

    updateProjectionMatrix();
}


void Camera::setPosition(glm::vec3 position) {
    Orientable::setPosition(position);
    updateViewMatrix();
}

void Camera::setOrientation(glm::quat orientation) {
    Orientable::setOrientation(orientation);
    updateViewMatrix();
}

void Camera::setScale(glm::vec3 newScale) {
    Orientable::setScale(newScale);
    updateViewMatrix();
}

void Camera::translate(glm::vec3 translation) {
    Orientable::translate(translation);
    updateViewMatrix();
}

void Camera::changeScale(glm::vec3 scaleFactor) {
    Orientable::changeScale(scaleFactor);
    updateViewMatrix();
}

void Camera::rotate(glm::quat rotation) {
    Orientable::rotate(rotation);
    updateViewMatrix();
}

void Camera::rotate(float angle, glm::vec3 axis) {
    Orientable::rotate(angle, axis);
    updateViewMatrix();
}

void Camera::look(float pitch, float yaw) {
    quat pitchRotation = angleAxis(pitch, vec3(1,0,0));
    quat yawRotation = angleAxis(yaw, vec3(0,1,0));

    orientation = yawRotation * orientation * pitchRotation;

    if (dot(up(), vec3(0,1,0)) <= 0) {
        vec3 newForward = vec3(0,1,0);
        if (dot(forward(), vec3(0,-1,0)) > 0.99) {
            newForward = vec3(0,-1,0);
        }

        orientation = normalize(quatLookAt(newForward, up()));
    }

    updateViewMatrix();
}

void Camera::lookAt(glm::vec3 target, glm::vec3 up, glm::vec3 alternativeUp) {
    Orientable::lookAt(target, up, alternativeUp);
    updateViewMatrix();
}
