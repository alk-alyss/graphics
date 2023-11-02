#include <renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    VPLocation = glGetUniformLocation(shader.getProgram(), "vp");
    ModelLocation = glGetUniformLocation(shader.getProgram(), "model");
}

void Renderer::render(Camera& camera, std::vector<std::shared_ptr<Mesh>> meshList) {
    glUseProgram(shader.getProgram());

    glm::mat4 VP = camera.getVP();
    glUniformMatrix4fv(VPLocation, 1, GL_FALSE, &VP[0][0]);

    for(auto& mesh : meshList) {
        glm::mat4 modelMatrix = mesh->modelMatrix();

        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        mesh->draw();
    }

    glUseProgram(0);
}