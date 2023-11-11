#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    private:
        float nearCP;
        float farCP;

        float fov;
        float aspectRatio;

        glm::vec3 position;
        glm::vec3 rotation; // tilt, yaw, roll

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        glm::mat4 VP;

    protected:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 getDirectionVector();
        glm::vec3 getUpVector();
        glm::vec3 getRightVector();

    public:
        Camera();
        glm::mat4 getVP() {return VP;}

        void updateAspectRatio(GLFWwindow* window);

        void translate(glm::vec3 translation);

        void rotate(float pitch, float yaw, float roll);
        void rotate(float pitch, float yaw);

        void zoom(float amount);
};

#endif