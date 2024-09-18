#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

Renderer::Renderer(
    std::shared_ptr<Shader> singleShader,
    std::shared_ptr<Shader> instancedShader
) : singleShader(singleShader), instancedShader(instancedShader) {
    // Allocate memory for view and projection matrices
    glGenBuffers(1, &matricesUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);

    // Allocate memory for lights
    glGenBuffers(1, &lightsUBO);

    lightsUBOsize = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();
    lightsUBOsize += MAX_POINT_LIGHTS * PointLight::sizeofData();
    lightsUBOsize += 2 * sizeof(GLuint); // dirLightsCount, pointLightsCount

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, lightsUBOsize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsUBO);
}

void Renderer::render(
    const Scene& scene
) {
    glm::mat4 initialTransformation{1};

    // Draw maze
    instancedShader->bind();

    uploadMatrices(scene.player->getCamera());
    uploadLights(scene);

    // Upload camera position
    glm::vec3 cameraPosition = scene.player->getCamera().getPosition();
    glUniform3fv(instancedShader->getCameraPositionLocation(), 1, glm::value_ptr(cameraPosition));

    scene.maze->draw(initialTransformation, instancedShader);

    // Draw rest of the scene
    singleShader->bind();

    // Upload camera position
    glUniform3fv(singleShader->getCameraPositionLocation(), 1, glm::value_ptr(cameraPosition));

    // Draw floor
    scene.floor->draw(initialTransformation, singleShader);

    // Draw player
    scene.player->draw(initialTransformation, singleShader);

    // Draw portals
    if (scene.portals.first != nullptr) scene.portals.first->draw(initialTransformation, singleShader);
    if (scene.portals.second != nullptr) scene.portals.second->draw(initialTransformation, singleShader);

    glUseProgram(0);
}

void Renderer::uploadMatrices(const Camera& camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::uploadLights(const Scene& scene) {
    std::vector<DirectionalLight> directionalLights = scene.directionalLights;
    std::vector<PointLight> pointLights = scene.pointLights;

    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);

    // Upload directional lights
    size_t offset = 0;
    for (auto& dirLight : directionalLights) {
        std::vector<glm::vec4> lightData = dirLight.data();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    // Upload point lights
    offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();
    for (auto& pointLight : pointLights) {
        std::vector<glm::vec4> lightData = pointLight.data();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    // Upload directional lights count
    offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData() + MAX_POINT_LIGHTS * PointLight::sizeofData();
    GLuint dirLightsCount = directionalLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dirLightsCount * sizeof(dirLightsCount), &dirLightsCount);

    // Upload point lights count
    offset += sizeof(dirLightsCount);
    GLuint pointLightsCount = pointLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, pointLightsCount * sizeof(pointLightsCount), &pointLightsCount);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
