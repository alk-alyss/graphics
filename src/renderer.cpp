#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

Renderer::Renderer(std::shared_ptr<Shader> shader) : shader(shader) {
    glGenBuffers(1, &matricesUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);

    glGenBuffers(1, &lightsUBO);

    lightsUBOsize = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();
    lightsUBOsize += MAX_POINT_LIGHTS * PointLight::sizeofData();
    lightsUBOsize += 2 * sizeof(GLuint);

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(
        GL_UNIFORM_BUFFER,
        lightsUBOsize,
        NULL, GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUBO);
}

void Renderer::render(
    const Scene& scene
) {
    shader->bind();

    glm::vec3 cameraPosition = scene.player->getCamera().getPosition();
    glUniform3fv(shader->getCameraPositionLocation(), 1, glm::value_ptr(cameraPosition));

    uploadMatrices(scene.player->getCamera());
    uploadLights(scene);

    glm::mat4 initialTransformation{1};

    for(auto& model : scene.models) {
        model->draw(initialTransformation, shader);
    }

    scene.player->draw(initialTransformation, shader);

    glUseProgram(0);
}

void Renderer::uploadMatrices(const Camera& camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::uploadLights(
    const Scene& scene
) {
    std::vector<DirectionalLight> directionalLights = scene.directionalLights;
    std::vector<PointLight> pointLights = scene.pointLights;

    size_t offset = 0;

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);

    for (auto& dirLight : directionalLights) {
        std::vector<glm::vec4> lightData = dirLight.data();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();

    for (auto& pointLight : pointLights) {
        std::vector<glm::vec4> lightData = pointLight.data();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData() + MAX_POINT_LIGHTS * PointLight::sizeofData();

    GLuint dirLightsCount = directionalLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dirLightsCount * sizeof(dirLightsCount), &dirLightsCount);
    offset += sizeof(dirLightsCount);

    GLuint pointLightsCount = pointLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, pointLightsCount * sizeof(pointLightsCount), &pointLightsCount);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
