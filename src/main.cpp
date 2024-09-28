#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "context.hpp"
#include "orientable.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "controls.hpp"
#include "collisions.hpp"
#include "resources.hpp"
#include "maze.hpp"

std::vector<Material> loadMaterials() {
    std::cout << "Loading materials..." << std::endl;

    grassMaterial = Material("resources/textures/whispy-grass-meadow-bl");
    dirtMaterial = Material("resources/textures/dry-dirt2-bl");
    metalMaterial = Material("resources/textures/rusty-metal-bl");

    scuffedPlasticMaterial = Material("resources/textures/scuffed-plastic-yellow-bl");
    bluePortalMaterial = scuffedPlasticMaterial;
    orangePortalMaterial = scuffedPlasticMaterial;

    bluePortalMaterial.albedo = std::make_shared<Texture>(std::string("resources/textures/scuffed-plastic-blue-bl/scuffed-plastic8-alb.png"), true);
    orangePortalMaterial.albedo = std::make_shared<Texture>(std::string("resources/textures/scuffed-plastic-orange-bl/scuffed-plastic4-alb.png"), true);

    std::vector<Material> materials;
    materials.push_back(grassMaterial);
    materials.push_back(dirtMaterial);
    materials.push_back(metalMaterial);

    std::cout << " done" << std::endl;

    return materials;
}

void loadMeshes() {
    std::cout << "Loading meshes...";
    planeMesh = Mesh::plane();
    cubeMesh = std::make_shared<Mesh>("resources/models/cube.obj");
    suzanneMesh = std::make_shared<Mesh>("resources/models/suzanne.obj");
    std::cout << " done" << std::endl;
}

int main(void) {
    std::srand(std::time(nullptr));

    try {
        std::cout << "Portal Maze" << std::endl;
        const std::shared_ptr<GLFWwindow> window = createWindow();
        Renderer renderer;

        auto materials = loadMaterials();
        loadMeshes();

        std::cout << "Creating floor...";
        const std::shared_ptr<Model> floor = std::make_shared<Model>(
                planeMesh,
                dirtMaterial
            );

        floor->setScale(glm::vec3(100));
        std::cout << " done" << std::endl;

        std::cout << "Creating directional lights...";
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
        std::cout << " done" << std::endl;

        std::cout << "Creating point lights...";
        std::vector<PointLight> pointLights{
            PointLight(
                glm::vec3(0,3,10)
            )
        };
        std::cout << " done" << std::endl;

        std::cout << "Creating player...";
        std::shared_ptr<Player> player = std::make_shared<Player>(
            std::make_shared<Model>(suzanneMesh, scuffedPlasticMaterial),
            glm::vec3(0.0f, 1.5f, 10.0f),
            DEFAULT_ORIENTATION,
            glm::vec3(0.5f)
        );
        std::cout << " done" << std::endl;

        std::cout << "Creating maze...";
        std::shared_ptr<Maze> maze = std::make_shared<Maze>(19, 19, materials);
        std::cout << " done" << std::endl;

        std::cout << "Creating scene...";
        Scene scene(
            player,
            maze,
            floor,
            maze->getColliders(),
            dirLights,
            pointLights
        );
        std::cout << " done" << std::endl;

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float lastTime = glfwGetTime();

        // Main loop
        std::cout << "Starting main loop" << std::endl;
        do {
            float currentTime = glfwGetTime();
            float deltaTime = currentTime - lastTime;

            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // Events
            glfwPollEvents();

            handleMouse(window, scene, deltaTime);
            handleKeyboard(window, scene, deltaTime);

            if (!player->isNoClip()) checkCollisions(scene, deltaTime);

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
