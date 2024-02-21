#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "context.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "controls.hpp"
#include "collisions.hpp"

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

std::vector<Material> loadMaterials() {
    std::vector<Material> materials;
    materials.push_back(Material("resources/textures/whispy-grass-meadow-bl"));
    materials.push_back(Material("resources/textures/dry-dirt2-bl"));
    materials.push_back(Material("resources/textures/rusty-metal-bl"));
    return materials;
}

std::vector<std::shared_ptr<Model>> generateMaze(
        std::vector<std::pair<int, int>> mazeMap,
        std::vector<Material> materials
    ) {
    const std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("resources/models/cube.obj");

    std::vector<std::shared_ptr<Model>> maze;

    float scalling = 3;

    for (int i=0; i<mazeMap.size(); i++) {
        auto pair = mazeMap[i];

        const std::shared_ptr<Model> cube = std::make_shared<Model>(
                cubeMesh,
                materials[i % materials.size()]
            );

        cube->setPosition(scalling * pair.first, scalling/2, -scalling * pair.second);
        cube->setScale(glm::vec3(scalling/2, scalling/2, scalling/2));

        maze.push_back(cube);
    }

    return maze;
}

const std::shared_ptr<Model> loadSuzanne(Material material) {
    const std::shared_ptr<Mesh> suzanneMesh = std::make_shared<Mesh>("resources/models/suzanne.obj");
    const Transformation suzanneTransformation = Transformation(
            glm::vec3(0, 0, -0.2),
            glm::vec3(0, glm::radians(180.0), 0),
            glm::vec3(0.9)
        );

    return std::make_shared<Model>(suzanneMesh, material, suzanneTransformation);
}

const std::shared_ptr<Model> playerCollider(Material material) {
    const std::shared_ptr<Mesh> playerCollider = std::make_shared<Mesh>("resources/models/earth.obj");
    Transformation colliderTransformation;
    colliderTransformation.setScale(0.5);

    return std::make_shared<Model>(playerCollider, material, colliderTransformation);
}

int main(void) {
    try {
        const std::shared_ptr<GLFWwindow> window = createWindow();

        auto materials = loadMaterials();

        std::vector<std::shared_ptr<Model>> models;

        const std::shared_ptr<Mesh> plane = Mesh::plane();
        const std::shared_ptr<Model> planeModel = std::make_shared<Model>(
                plane,
                materials[1]
            );

        planeModel->setScale(glm::vec3(100));

        models.push_back(planeModel);

        auto mazeModels = generateMaze(mazeMap, materials);
        models.insert(models.end(), mazeModels.begin(), mazeModels.end());

        std::vector<DirectionalLight> dirLights{
            DirectionalLight(
                glm::vec3(5,5,10),
                glm::vec3(glm::radians(-45.0),glm::radians(45.0),0)
            ),
            DirectionalLight(
                glm::vec3(5,5,10),
                glm::vec3(glm::radians(-45.0),glm::radians(180.0+45),0)
            ),
        };

        std::vector<PointLight> pointLights{
            PointLight(
                glm::vec3(0,3,10)
            )
        };

        std::shared_ptr<Player> player = std::make_shared<Player>(
                loadSuzanne(materials[0]),
                playerCollider(materials[0]),
                glm::vec3(0.0f, 2.0f, 10.0f)
            );

        const Scene scene(
            player,
            models,
            dirLights,
            pointLights
        );

        std::shared_ptr<Shader> forwardPBR = std::make_shared<Shader>("shaders/pbr.vert", "shaders/pbr.frag");
        Renderer renderer(forwardPBR);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        do {
            float currentTime = glfwGetTime();
            float deltaTime = currentTime - lastTime;

            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Events
            glfwPollEvents();

            handleMouse(window, player, deltaTime);
            handleKeyboard(window, player, deltaTime);

            checkCollisions(scene, deltaTime);

            player->updateCamera(window.get());
            player->update(deltaTime);

            renderer.render(scene);

            // Swap buffers
            glfwSwapBuffers(window.get());

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
