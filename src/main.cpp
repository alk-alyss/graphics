// Include C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <context.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <renderer.hpp>

int main(void) {
    try {
        GLFWwindow* window = createWindow();

        // Create and compile our GLSL program from the shaders
        glEnable(GL_PROGRAM_POINT_SIZE);
        Shader shader("shaders/simple.vert", "shaders/simple.frag");

        std::vector<std::shared_ptr<Mesh>> meshList;
        meshList.push_back(std::make_shared<Mesh>());

        Camera camera;

        Renderer renderer(shader);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        do {
            // Clear the screen.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.updateAspectRatio(window);

            renderer.render(camera, meshList);

            // Swap buffers
            glfwSwapBuffers(window);

            // Events
            glfwPollEvents();
        } // Check if the ESC key was pressed or the window was closed
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
