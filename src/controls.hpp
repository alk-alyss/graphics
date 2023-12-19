#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <memory>
#include <GLFW/glfw3.h>

#include "camera.hpp"

void handleKeyboard(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Camera> camera, float deltaTime);

void handleMouse(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Camera> camera, float deltaTime);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
