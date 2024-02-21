#include "controls.hpp"

#include <iostream>

void handleMouse(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Player> player, float deltaTime) {
    GLFWwindow* window = windowPtr.get();

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    mouseX -= width/2.0;
    mouseY -= height/2.0;

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2.0, height / 2.0);

    player->look(mouseX, mouseY, deltaTime);
}

void handleKeyboard(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Player> player, float deltaTime) {
    GLFWwindow* window = windowPtr.get();

    // Move forware - backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player->moveForward(deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player->moveBackward(deltaTime);
    }
    // Strafe right - left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->moveRight(deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->moveLeft(deltaTime);
    }

    // Zoom in - out
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player->zoom(deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player->zoom(-deltaTime);
    }

    static bool toggle = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!toggle) player->toggleNoClip();
        toggle = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        toggle = false;
    }
}
