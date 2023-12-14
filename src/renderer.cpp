#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    MVPLocation = glGetUniformLocation(shader.getProgram(), "mvp");
}

void Renderer::render(Camera& camera, std::vector<std::shared_ptr<Mesh>> meshList) {
    glUseProgram(shader.getProgram());

    glm::mat4 VP = camera.getVP();

    for(auto& mesh : meshList) {
        glm::mat4 modelMatrix = mesh->modelMatrix();

        glm::mat4 mvp = VP * modelMatrix;

        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);

        mesh->draw();
    }

    glUseProgram(0);
}
