#include <renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    viewMatrix = glm::lookAt(glm::vec3(5, 3, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);

    VP = projectionMatrix * viewMatrix;

    VPLocation = glGetUniformLocation(shader.getProgram(), "vp");
    ModelLocation = glGetUniformLocation(shader.getProgram(), "model");
}

void Renderer::render(std::vector<std::shared_ptr<Mesh>> meshList) {
    glUseProgram(shader.getProgram());
    glUniformMatrix4fv(VPLocation, 1, GL_FALSE, &VP[0][0]);

    for(auto& mesh : meshList) {
        glm::mat4 modelMatrix = mesh->modelMatrix();

        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        mesh->draw();
    }

    glUseProgram(0);
}