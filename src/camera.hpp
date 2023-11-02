#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    private:
        float fov;
        float nearCP;
        float farCP;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        glm::mat4 VP;

    public:
        Camera();
        glm::mat4 getVP() {return VP;}

        void updateAspectRatio(GLFWwindow* window);
        void translate(glm::vec3 translation);
        void rotate(float angle, glm::vec3 axis);
};

#endif