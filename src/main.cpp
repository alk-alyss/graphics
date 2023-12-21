#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "context.hpp"
#include "shader.hpp"
#include "cube.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "controls.hpp"

int main(void) {
    try {
        std::shared_ptr<GLFWwindow> window = createWindow();

        // Create and compile our GLSL program from the shaders
        glEnable(GL_PROGRAM_POINT_SIZE);
        Shader shader("shaders/simple.vert", "shaders/simple.frag");

        std::vector<std::shared_ptr<Mesh>> meshList;
        std::shared_ptr<Mesh> suzanne = std::make_shared<Mesh>("resources/models/suzanne.obj");
        meshList.push_back(suzanne);

        std::shared_ptr<Camera> camera = std::make_shared<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

        DirectionLight light(
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                10,
                glm::vec3(0,5,10),
                glm::vec3(glm::radians(180.0),0,0)
            );

        Renderer renderer(shader);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        do {
            float currentTime = glfwGetTime();
            float deltaTime = currentTime - lastTime;

            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera->updateAspectRatio(window.get());

            renderer.render(*camera, meshList, light, gold);

            // Swap buffers
            glfwSwapBuffers(window.get());

            // Events
            glfwPollEvents();

            handleMouse(window, camera, deltaTime);
            handleKeyboard(window, camera, deltaTime);

            lastTime = currentTime;
        } // Check if the window should be closed
        while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(window.get()) == 0);

        glfwTerminate();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        getchar();
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
