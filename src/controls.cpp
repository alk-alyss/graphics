#include "controls.hpp"
#include "collisions.hpp"

void handleMouse(const std::shared_ptr<GLFWwindow> windowPtr, Scene& scene, const float deltaTime) {
    GLFWwindow* window = windowPtr.get();
    auto& player = scene.player;

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    mouseX -= width/2.0;
    mouseY -= height/2.0;

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2.0, height / 2.0);

    player->look(mouseX, mouseY, deltaTime);

    static bool mouse1Toggle = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (!mouse1Toggle) {
            glm::vec3 normalVector;
            auto model = castRay(scene, player->getPosition(), player->forward(), normalVector);
            if (model == nullptr) return;
            scene.createFirstPortal(model, normalVector);
        }
        mouse1Toggle = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        mouse1Toggle = false;
    }

    static bool mouse2Toggle = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        if (!mouse2Toggle) {
            glm::vec3 normalVector;
            auto model = castRay(scene, player->getPosition(), player->forward(), normalVector);
            if (model == nullptr) return;
            scene.createSecondPortal(model, normalVector);
        }
        mouse2Toggle = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        mouse2Toggle = false;
    }
}

void handleKeyboard(const std::shared_ptr<GLFWwindow> windowPtr, const Scene& scene, const float deltaTime) {
    GLFWwindow* window = windowPtr.get();
    auto& player = scene.player;

    // Move forward - backward (W - S)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player->moveForward();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player->moveBackward();
    }
    // Strafe right - left (D - A)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->moveRight();
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->moveLeft();
    }

    // Zoom in - out (UP - DOWN)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player->zoom(deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player->zoom(-deltaTime);
    }

    // Toggle no-clip mode (SPACE)
    static bool toggle = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!toggle) player->toggleNoClip();
        toggle = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        toggle = false;
    }
}
