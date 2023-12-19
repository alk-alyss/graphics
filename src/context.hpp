#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

void captureMouse(GLFWwindow* window);
void releaseMouse(GLFWwindow* window) ;

struct DestroyGLFWwindow{

    void operator()(GLFWwindow* ptr){
         glfwDestroyWindow(ptr);
    }

};

std::shared_ptr<GLFWwindow> createWindow();

#endif
