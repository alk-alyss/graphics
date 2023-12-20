#include "controls.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

void handleMouse(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Camera> camera, float deltaTime) {
    GLFWwindow* window = windowPtr.get();

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    mouseX -= width/2.0;
    mouseY -= height/2.0;

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2.0, height / 2.0);

    camera->look(mouseX, mouseY, deltaTime);
}

void handleKeyboard(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Camera> camera, float deltaTime) {
    GLFWwindow* window = windowPtr.get();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->moveForward(deltaTime);
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->moveBackward(deltaTime);
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->moveRight(deltaTime);
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->moveLeft(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera->zoom(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera->zoom(-deltaTime);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
