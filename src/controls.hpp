#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "scene.hpp"

#include <memory>
#include <GLFW/glfw3.h>

void handleMouse(const std::shared_ptr<GLFWwindow> windowPtr, Scene& scene, const float deltaTime);

void handleKeyboard(const std::shared_ptr<GLFWwindow> windowPtr, const Scene& scene, const float deltaTime);

#endif
