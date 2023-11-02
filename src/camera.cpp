#include <camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    viewMatrix = glm::lookAt(glm::vec3(5, 3, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);

    VP = projectionMatrix * viewMatrix;
}

void Camera::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

    VP = projectionMatrix * viewMatrix;
}