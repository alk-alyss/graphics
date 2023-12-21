#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    glGenBuffers(1, &matricesUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);

    glGenBuffers(1, &lightsUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, 1 * Light::sizeofData(), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUBO);
}

void Renderer::render(
    const Camera& camera,
    const std::vector<std::shared_ptr<Mesh>>& meshList,
    const Light& light,
    const Material& material
) {
    glUseProgram(shader.getProgram());

    uploadMatrices(camera);
    uploadLights(light);
    uploadMaterial(material);

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

void Renderer::uploadLights(const Light& light) {
    /* glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO); */
    /* std::vector<glm::vec4> lightData = light.data(); */

    /* glBufferSubData(GL_UNIFORM_BUFFER, 0, Light::sizeofData(), lightData.data()); */
    /* glBindBuffer(GL_UNIFORM_BUFFER, 0); */


    std::vector<glm::vec4> lightData = light.data();
    glUniform4fv(shader.LaLocation, 1, glm::value_ptr(lightData[0]));
    glUniform4fv(shader.LdLocation, 1, glm::value_ptr(lightData[1]));
    glUniform4fv(shader.LsLocation, 1, glm::value_ptr(lightData[2]));
    glUniform3fv(shader.lightPositionLocation, 1, glm::value_ptr(lightData[3]));
}

void Renderer::uploadMaterial(const Material& mtl) {
    glUniform4f(shader.KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(shader.KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(shader.KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(shader.NsLocation, mtl.Ns);
}
