#include <renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    viewMatrix = glm::lookAt(glm::vec3(5, 3, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 15.0f);

    VP = projectionMatrix * viewMatrix;

    MVPLocation = glGetUniformLocation(shader.getProgram(), "mvp");
}

void Renderer::render(std::vector<std::shared_ptr<Mesh>> meshList) {
    glUseProgram(shader.getProgram());

    for(auto& mesh : meshList) {
        glm::mat4 modelMatrix = mesh->modelMatrix();

        glm::mat4 MVP = VP * modelMatrix;

        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

        mesh->draw();
    }

    glUseProgram(0);
}