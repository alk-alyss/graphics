#include <cstdlib>
#include <ctime>
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
#include "resources.hpp"
#include "maze.hpp"

std::vector<Material> loadMaterials() {
    grassMaterial = Material("resources/textures/whispy-grass-meadow-bl");
    dirtMaterial = Material("resources/textures/dry-dirt2-bl");
    metalMaterial = Material("resources/textures/rusty-metal-bl");

    std::vector<Material> materials;
    materials.push_back(grassMaterial);
    materials.push_back(dirtMaterial);
    materials.push_back(metalMaterial);
    return materials;
}

void loadMeshes() {
    planeMesh = Mesh::plane();
    sphereMesh = std::make_shared<Mesh>("resources/models/earth.obj");
    cubeMesh = std::make_shared<Mesh>("resources/models/cube.obj");
    suzanneMesh = std::make_shared<Mesh>("resources/models/suzanne.obj");
}

const std::shared_ptr<Model> loadSuzanne(Material material) {
    const Transformation suzanneTransformation = Transformation(
            glm::vec3(0, 0, -0.2),
            eulerToQuat(0, glm::radians(180.0), 0),
            glm::vec3(0.5)
        );

    return std::make_shared<Model>(suzanneMesh, material, suzanneTransformation);
}

const std::shared_ptr<Model> playerCollider(Material material) {
    Transformation colliderTransformation;
    colliderTransformation.setScale(0.3);

    return std::make_shared<Model>(sphereMesh, material, colliderTransformation);
}

int main(void) {
    std::srand(std::time(nullptr));

    try {
        std::cout << "Portal Maze" << std::endl;
        const std::shared_ptr<GLFWwindow> window = createWindow();

        std::shared_ptr<Shader> singleShader = std::make_shared<Shader>("shaders/pbr.vert", "shaders/pbr.frag");
        std::shared_ptr<Shader> instancedShader = std::make_shared<Shader>("shaders/instanced.vert", "shaders/pbr.frag");
        Renderer renderer(singleShader, instancedShader);

        auto materials = loadMaterials();
        loadMeshes();

        std::vector<std::shared_ptr<Model>> models;

        const std::shared_ptr<Model> planeModel = std::make_shared<Model>(
                planeMesh,
                dirtMaterial
            );

        planeModel->setScale(glm::vec3(100));

        models.push_back(planeModel);

        std::shared_ptr<Maze> maze = std::make_shared<Maze>(19, 19, grassMaterial);

        std::vector<DirectionalLight> dirLights{
            DirectionalLight(
                glm::vec3(5,5,10),
                eulerToQuat(glm::radians(-45.0),glm::radians(45.0),0)
            ),
            DirectionalLight(
                glm::vec3(5,5,10),
                eulerToQuat(glm::radians(-45.0),glm::radians(180.0+45),0)
            ),
        };

        std::vector<PointLight> pointLights{
            PointLight(
                glm::vec3(0,3,10)
            )
        };

        std::shared_ptr<Player> player = std::make_shared<Player>(
            loadSuzanne(metalMaterial),
            playerCollider(grassMaterial),
            glm::vec3(0.0f, 2.0f, 10.0f)
        );

        Scene scene(
            player,
            maze,
            models,
            dirLights,
            pointLights
        );

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        // Main loop
        do {
            float currentTime = glfwGetTime();
            float deltaTime = currentTime - lastTime;

            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Events
            glfwPollEvents();

            handleMouse(window, scene, deltaTime);
            handleKeyboard(window, scene, deltaTime);

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
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
