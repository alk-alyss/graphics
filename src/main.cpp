// Include C++ headers
#include <iostream>
#include <string>

#include <context.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>

#include <mesh.hpp>

// Global variables
GLuint shaderProgram;
Mesh mesh;

void free() {
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop() {
    do {
        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        mesh.render();

        // Swap buffers
        glfwSwapBuffers(window);

        // Events
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}


int main(void) {
    try {
        initializeContext();

        // Create and compile our GLSL program from the shaders
        glEnable(GL_PROGRAM_POINT_SIZE);
        shaderProgram = loadShaders("shaders/simple.vert", "shaders/simple.frag");

        mesh.assignShader(shaderProgram);
        mesh.loadVram();

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        mainLoop();
        free();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}
