#include <camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

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