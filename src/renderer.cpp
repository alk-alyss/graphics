#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
   glGenBuffers(1, &matricesUBO);

   glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
   glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_UNIFORM_BUFFER, 0);

   glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
}

void Renderer::render(const Camera& camera, const std::vector<std::shared_ptr<Mesh>> meshList) {
    glUseProgram(shader.getProgram());

    uploadMatrices(camera);

    for(auto& mesh : meshList) {
        glUniformMatrix4fv(shader.getMLocation(), 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix()));

        mesh->draw();
    }

    glUseProgram(0);
}

void Renderer::uploadMatrices(const Camera& camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
