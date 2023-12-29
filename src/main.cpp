#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "context.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "controls.hpp"

std::vector<std::pair<int, int>> mazeMap{
    std::pair<int, int> (0,0),
    std::pair<int, int> (0,1),
    std::pair<int, int> (0,2),
    std::pair<int, int> (0,3),
    std::pair<int, int> (0,4),
    std::pair<int, int> (1,2),
    std::pair<int, int> (1,4),
    std::pair<int, int> (2,0),
    std::pair<int, int> (2,2),
    std::pair<int, int> (2,4),
    std::pair<int, int> (3,0),
    std::pair<int, int> (4,0),
    std::pair<int, int> (4,1),
    std::pair<int, int> (4,2),
    std::pair<int, int> (4,3),
    std::pair<int, int> (4,4)
};

std::vector<std::shared_ptr<Material>> loadMaterials() {
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(std::make_shared<Material>("resources/textures/whispy-grass-meadow-bl"));
    /* materials.push_back(std::make_shared<Material>("resources/textures/Grass001_2K-PNG")); */
    return materials;
}

std::vector<std::shared_ptr<Model>> generateMaze(
        std::vector<std::pair<int, int>> mazeMap,
        std::vector<std::shared_ptr<Material>> materials
    ) {
    const std::shared_ptr<Node> cubeMesh = std::make_shared<Mesh>("resources/models/cube.obj");

    std::vector<std::shared_ptr<Model>> maze;

    float scalling = 2.5;

    for (int i=0; i<mazeMap.size(); i++) {
        auto pair = mazeMap[i];

        const std::shared_ptr<Model> cube = std::make_shared<Model>(
                cubeMesh,
                materials[i % materials.size()],
                glm::vec3(scalling * pair.first, scalling/2, -scalling * pair.second),
                glm::vec3(0, 0, 0),
                glm::vec3(scalling/2, scalling/4, scalling/2)
            );

        maze.push_back(cube);
    }

    return maze;
}

int main(void) {
    try {
        const std::shared_ptr<GLFWwindow> window = createWindow();

        /* const std::shared_ptr<Node> suzanneMesh = std::make_shared<Mesh>("resources/models/suzanne.obj"); */
        /* const std::shared_ptr<Model> suzanne = std::make_shared<Model>(suzanneMesh, grass); */

        std::vector<std::shared_ptr<Material>> materials = loadMaterials();

        std::vector<std::shared_ptr<Model>> models;

        const std::shared_ptr<Node> plane = Mesh::plane();
        const std::shared_ptr<Model> planeModel = std::make_shared<Model>(
                plane,
                materials[0],
                glm::vec3(0, 0.001, 0),
                glm::vec3(0, 0, 0),
                glm::vec3(100, 0, 100)
        );

        models.push_back(planeModel);

        auto mazeModels = generateMaze(mazeMap, materials);
        models.insert(models.end(), mazeModels.begin(), mazeModels.end());

        std::vector<DirectionalLight> dirLights{
            DirectionalLight(
                glm::vec4(1,1,1,1),
                10,
                glm::vec3(5,5,10),
                glm::vec3(glm::radians(-45.0),glm::radians(45.0),0)
            ),
            DirectionalLight(
                glm::vec4(1,1,1,1),
                10,
                glm::vec3(5,5,10),
                glm::vec3(glm::radians(-45.0),glm::radians(180.0+45),0)
            ),
        };

        const Scene scene(
            models,
            dirLights
        );

        std::shared_ptr<Camera> camera = std::make_shared<FirstPersonCamera>(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f));

        const Shader forwardPBR("shaders/pbr.vert", "shaders/pbr.frag");
        Renderer renderer(forwardPBR);

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
