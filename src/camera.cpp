#include "camera.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 rotation) {
    nearCP = 0.1f;
    farCP = 100.0f;

    fov = 45.0f;
    aspectRatio = 16.0f/9.0f;

    this->position = position;
    setRotation(rotation);

    updateViewMatrix();

    updateProjectionMatrix();
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + direction, up());

    VP = projectionMatrix * viewMatrix;
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearCP, farCP);

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
