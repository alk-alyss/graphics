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
        const std::shared_ptr<GLFWwindow> window = createWindow();

        const std::shared_ptr<Node> suzanneMesh = std::make_shared<Mesh>("resources/models/suzanne.obj");
        const std::shared_ptr<Texture> suzanneDiffuse = std::make_shared<Texture>("resources/textures/suzanne_diffuse.bmp");
        const std::shared_ptr<Texture> suzanneSpecular = std::make_shared<Texture>("resources/textures/suzanne_diffuse.bmp");

        const std::shared_ptr<Material> suzanneMaterial = std::make_shared<Material>(
            suzanneDiffuse,
            suzanneSpecular,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );

        std::vector<std::shared_ptr<Model>> models;
        models.push_back(std::make_shared<Model>(suzanneMesh, suzanneMaterial));

        std::vector<DirectionalLight> dirLights;
        dirLights.push_back(
            DirectionalLight(
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                glm::vec4(1,1,1,1),
                10,
                glm::vec3(0,5,10),
                glm::vec3(glm::radians(180.0),0,0)
            )
        );

        const Scene scene(
            models,
            dirLights
        );

        std::shared_ptr<Camera> camera = std::make_shared<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

        Shader shader("shaders/simple.vert", "shaders/simple.frag");
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

            renderer.render(*camera, scene);

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
