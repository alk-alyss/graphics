#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "player.hpp"

#include <memory>
#include <GLFW/glfw3.h>

void handleKeyboard(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Player> player, float deltaTime);

void handleMouse(std::shared_ptr<GLFWwindow> windowPtr, std::shared_ptr<Player> player, float deltaTime);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
