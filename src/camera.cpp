#include <camera.hpp>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    nearCP = 0.1f;
    farCP = 100.0f;

    fov = 45.0f;
    aspectRatio = 16.0f/9.0f;

    position = glm::vec3(0, 0, -10);
    rotation = glm::vec3(0, 0, 0);

    updateViewMatrix();
    updateProjectionMatrix();
}

glm::vec3 Camera::getDirectionVector() {
    glm::vec3 direction(
        cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)),
        sin(glm::radians(rotation.x)),
        -sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x))
    );
    return direction;
}

glm::vec3 Camera::getUpVector() {
    return glm::vec3(0, 1, 0);
}

glm::vec3 Camera::getRightVector() {
    glm::vec3 direction = getDirectionVector();
    glm::vec3 up = getUpVector();

    return glm::cross(direction, up);
}

void Camera::updateViewMatrix() {
    glm::vec3 direction = getDirectionVector();

    glm::vec3 up = getUpVector();

    viewMatrix = glm::lookAt(position, position + direction, up);

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
}

void Camera::translate(glm::vec3 translation) {
    position += translation;

    updateViewMatrix();
}

void Camera::rotate(float pitch, float yaw, float roll) {
    rotation.x += pitch;
    rotation.y += yaw;
    rotation.z += roll;

    updateViewMatrix();
}

void Camera::rotate(float pitch, float yaw) {
    rotate(pitch, yaw, 0);
}

void Camera::zoom(float amount) {
    fov += amount;

    updateProjectionMatrix();
}