#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

Renderer::Renderer(const Shader& shader) : shader(shader) {
    glGenBuffers(1, &matricesUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);

    glGenBuffers(1, &lightsUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, MAX_DIR_LIGHTS * DirectionalLight::sizeofData(), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUBO);
}

void Renderer::render(
    const Scene& scene
) {
    glUseProgram(shader.getProgram());

    uploadMatrices(scene.player->getCamera());
    uploadLights(scene.directionalLights, scene.pointLights);

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
    const std::vector<DirectionalLight> directionalLights,
    const std::vector<PointLight> pointLights
) {
    std::vector<glm::vec4> lightData;
    for (auto& dirLight : directionalLights) {
        auto nextLightData = dirLight.data();
        lightData.insert(lightData.end(), nextLightData.begin(), nextLightData.end());
    }

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, lightData.size() * sizeof(lightData[0]), lightData.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
