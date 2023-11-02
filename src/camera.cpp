#include <camera.hpp>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    fov = 45.0f;
    nearCP = 0.1f;
    farCP = 100.0f;

    viewMatrix = glm::lookAt(glm::vec3(5, 3, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(fov), 16.0f/9.0f, nearCP, farCP);

    VP = projectionMatrix * viewMatrix;
}

void Camera::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(fov), (float)width/(float)height, nearCP, farCP);

    VP = projectionMatrix * viewMatrix;
}

void Camera::translate(glm::vec3 translation) {
    viewMatrix = glm::translate(viewMatrix, translation);

    VP = projectionMatrix * viewMatrix;
}

void Camera::rotate(float angle, glm::vec3 axis) {
    glm::vec3 cameraPosition = glm::inverse(viewMatrix)[3];

    viewMatrix = glm::translate(viewMatrix, cameraPosition);
    viewMatrix = glm::rotate(viewMatrix, angle, axis);
    viewMatrix = glm::translate(viewMatrix, -cameraPosition);

    VP = projectionMatrix * viewMatrix;
}