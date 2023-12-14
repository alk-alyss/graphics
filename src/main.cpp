#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "context.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "camera.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void) {
    try {
        GLFWwindow* window = createWindow();
        // glfwSetKeyCallback(window, keyCallback);

        // Create and compile our GLSL program from the shaders
        glEnable(GL_PROGRAM_POINT_SIZE);
        Shader shader("shaders/simple.vert", "shaders/simple.frag");

        std::vector<std::shared_ptr<Mesh>> meshList;
        meshList.push_back(std::make_shared<Mesh>());

        FirstPersonCamera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

        Renderer renderer(shader);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        do {
            float currentTime = glfwGetTime();
            float deltaTime = currentTime - lastTime;

            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.updateAspectRatio(window);

            renderer.render(camera, meshList);

            // Swap buffers
            glfwSwapBuffers(window);

            // Events
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera.moveForward(deltaTime);
            }
            // Move backward
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera.moveBackward(deltaTime);
            }
            // Strafe right
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera.strafeRight(deltaTime);
            }
            // Strafe left
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera.strafeLeft(deltaTime);
            }

            // Task 5.6: handle zoom in/out effects
            // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            //     camera.zoomIn(deltaTime);
            // }
            // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            //     camera.zoomOut(deltaTime);
            // }

            lastTime = currentTime;
        } // Check if the window should be closed
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0);

        glfwTerminate();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
