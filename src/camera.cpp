#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Camera::Camera(const vec3 position, const quat orientation) : Entity(position, orientation){
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::Camera(const vec3 position, const vec3 target, const vec3 up) {
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

void Camera::zoom(const float amount) {
    fov += amount;

    updateProjectionMatrix();
}


void Camera::setPosition(const glm::vec3 position) {
    Entity::setPosition(position);
    updateViewMatrix();
}

void Camera::setOrientation(const glm::quat orientation) {
    Entity::setOrientation(orientation);
    updateViewMatrix();
}

void Camera::setScale(const glm::vec3 newScale) {
    Entity::setScale(newScale);
    updateViewMatrix();
}

void Camera::translate(const glm::vec3 translation) {
    Entity::translate(translation);
    updateViewMatrix();
}

void Camera::changeScale(const glm::vec3 scaleFactor) {
    Entity::changeScale(scaleFactor);
    updateViewMatrix();
}

void Camera::rotate(const glm::quat rotation) {
    Entity::rotate(rotation);
    updateViewMatrix();
}

void Camera::rotate(const float angle, const glm::vec3 axis) {
    Entity::rotate(angle, axis);
    updateViewMatrix();
}

void Camera::look(const float pitch, const float yaw) {
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

void Camera::lookAt(const glm::vec3 target, const glm::vec3 up, const glm::vec3 alternativeUp) {
    Entity::lookAt(target, up, alternativeUp);
    updateViewMatrix();
}
