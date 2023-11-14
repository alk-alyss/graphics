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
constexpr glm::vec3 Camera::direction(glm::vec3 rotation) {
    glm::vec3 direction(
        glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)),
        glm::sin(glm::radians(rotation.x)),
        -glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x))
    );
    return direction;
}

constexpr glm::vec3 Camera::right(glm::vec3 rotation) {
    glm::vec3 right(
        glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.z)),
        glm::sin(glm::radians(rotation.z)),
        -glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.z))
    );

    return right;
}

constexpr glm::vec3 Camera::up(glm::vec3 rotation) {
    return glm::cross(right(rotation), direction(rotation));
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + direction(rotation), up(rotation));

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