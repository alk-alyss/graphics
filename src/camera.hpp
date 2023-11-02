#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        glm::mat4 VP;

    public:
        Camera();
        glm::mat4 getVP() {return VP;}

        void updateAspectRatio(GLFWwindow* window);
};

#endif