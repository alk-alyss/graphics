#include <camera.hpp>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 rotation) {
    nearCP = 0.1f;
    farCP = 100.0f;

    fov = 45.0f;
    aspectRatio = 16.0f/9.0f;

    this->position = position;
    this->rotation = rotation;

    updateViewMatrix();

    updateProjectionMatrix();
}

// Rotation (0, 0, 0) corresponds to directionVector (0, 0, -1)
glm::vec3 Camera::getDirectionVector() {
    glm::vec3 direction(
        glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)),
        glm::sin(glm::radians(rotation.x)),
        -glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x))
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

    updateProjectionMatrix();
}

void Camera::translate(glm::vec3 translation) {
    position += translation;

    updateViewMatrix();
}

void Camera::rotate(glm::vec3 rotation) {
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;

    updateViewMatrix();
}

// TODO: This is wrong fix it
void Camera::rotate(float angle, glm::vec3 axis) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);

    rotation = rotationMatrix * glm::vec4(rotation, 1.0f);

    updateViewMatrix();
}

void Camera::zoom(float amount) {
    fov += amount;

    updateProjectionMatrix();
}