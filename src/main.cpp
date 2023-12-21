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

Material gold = {
    glm::vec4(0.628281, 0.555802, 0.366065, 1.0),
    glm::vec4(0.75164, 0.60648, 0.22648, 1.0),
    glm::vec4(0.24725, 0.1995, 0.0745, 1.0),
    51.2
};

Material silver = {
    glm::vec4(0.508273, 0.508273, 0.508273, 1),
    glm::vec4(0.50754, 0.50754, 0.50754, 1),
    glm::vec4(0.19225, 0.19225, 0.19225, 1),
    51.2
};

Material copper = {
    glm::vec4(0.256777, 0.137622, 0.086014, 1),
    glm::vec4(0.7038, 0.27048, 0.0828, 1),
    glm::vec4(0.19125, 0.0735, 0.0225, 1),
    12.8
};

Material emerald = {
    glm::vec4(0.633, 0.727811, 0.633, 1),
    glm::vec4(0.07568, 0.61424, 0.07568, 1),
    glm::vec4(0.0215, 0.1745, 0.0215, 1),
    76.8
};

int main(void) {
    try {
        std::shared_ptr<GLFWwindow> window = createWindow();
        // glfwSetKeyCallback(window, keyCallback);

        // Create and compile our GLSL program from the shaders
        glEnable(GL_PROGRAM_POINT_SIZE);
        Shader shader("shaders/simple.vert", "shaders/simple.frag");

        std::vector<std::shared_ptr<Mesh>> meshList;
        std::shared_ptr<Mesh> suzanne = std::make_shared<Mesh>("resources/models/suzanne.obj");
        meshList.push_back(suzanne);

        std::shared_ptr<Camera> camera = std::make_shared<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

        Light light = Light(
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                10,
                glm::vec3(0,5,10),
                glm::vec3(0,0,0)
            );

        Renderer renderer(shader);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        /* meshList[0]->lookAt(glm::vec3(0,5,-5)); */

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
