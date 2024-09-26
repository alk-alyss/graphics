#include "renderer.hpp"
#include "shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

Renderer::Renderer() {
    singleShader = std::make_shared<Shader>("shaders/single.vert", "shaders/pbr.frag");
    instancedShader = std::make_shared<Shader>("shaders/instanced.vert", "shaders/pbr.frag");
    simpleShader = std::make_shared<Shader>("shaders/simple.vert", "shaders/simple.frag");

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

void Renderer::render(const Scene& scene) {
    uploadLights(scene);

    renderPortals(scene);
    renderScene(scene, scene.player->getCamera());

    glUseProgram(0);
}

void Renderer::renderScene(
    const Scene& scene,
    const Camera& camera,
    const bool drawPlayer,
    const bool drawPortals
) {
    uploadMatrices(camera);

    // Draw maze
    instancedShader->bind();

    // Upload camera position
    glm::vec3 cameraPosition = camera.getPosition();
    glUniform3fv(instancedShader->getCameraPositionLocation(), 1, glm::value_ptr(cameraPosition));

    scene.maze->draw(initialTransformation, instancedShader);

    // Draw rest of the scene
    singleShader->bind();

    // Upload camera position
    glUniform3fv(singleShader->getCameraPositionLocation(), 1, glm::value_ptr(cameraPosition));

    scene.floor->draw(initialTransformation, singleShader);
    if (drawPlayer || scene.player->isNoClip()) scene.player->draw(initialTransformation, singleShader);

    if (!drawPortals) return;
    for (auto& portal : scene.portals) {
        if (portal == nullptr) continue;

        glm::vec3 portalInitialScale = portal->getScale();
        portal->setScale(portalInitialScale * 1.05f);
        portal->draw(initialTransformation, singleShader);
        portal->setScale(portalInitialScale);
    }
}

void Renderer::renderPortals(const Scene& scene) {
    // Update portal cameras
    for (auto& portal : scene.portals) {
        if (portal != nullptr) portal->updateCamera(scene.player->getCamera());
    }

    // Enable stencil test
    glEnable(GL_STENCIL_TEST);

    for (auto& portal : scene.portals) {
        if (portal == nullptr) continue;

        // Draw portal frame if other portal does not exist
        uploadMatrices(scene.player->getCamera());
        singleShader->bind();
        if (portal->getLinkedPortal() == nullptr) {
            glm::vec3 portalInitialScale = portal->getScale();
            portal->setScale(portalInitialScale * 1.05f);
            portal->draw(initialTransformation, singleShader);
            portal->setScale(portalInitialScale);
            continue;
        }

        // Enable writing to stencil buffer
        glStencilMask(0xFF);

        // Clear stencil buffer
        glClear(GL_STENCIL_BUFFER_BIT);

        // Disable writing to color and depth buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        // Set stencil buffer to 1 where portal is drawn
        glStencilFunc(GL_NEVER, 1, 0xFF);
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

        // Draw portal frame to stencil buffer
        simpleShader->bind();
        portal->draw(initialTransformation, simpleShader);

        // Disable writing to stencil buffer
        glStencilMask(0x00);

        // Enable writing to color and depth buffer
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // Render only if stencil value is equal to 1
        glStencilFunc(GL_EQUAL, 1, 0xFF);

        // Draw view through portal
        renderScene(scene, portal->getLinkedPortal()->getCamera(), true, true);

        // Render only if stencil value is equal to 0
        glStencilFunc(GL_EQUAL, 0, 0xFF);

        // Draw portal frame
        uploadMatrices(scene.player->getCamera());
        singleShader->bind();
        glm::vec3 portalInitialScale = portal->getScale();
        portal->setScale(portalInitialScale * 1.05f);
        portal->draw(initialTransformation, singleShader);
        portal->setScale(portalInitialScale);
    }

    // Disable stencil test
    glDisable(GL_STENCIL_TEST);

    // Clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Disable writing to color buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Draw portals to depth buffer to prevent rendering objects behind them
    uploadMatrices(scene.player->getCamera());
    simpleShader->bind();
    for (auto& portal : scene.portals) {
        if (portal == nullptr) continue;

        glm::vec3 portalInitialScale = portal->getScale();
        portal->setScale(portalInitialScale * 1.05f);
        portal->draw(initialTransformation, simpleShader);
        portal->setScale(portalInitialScale);
    }

    // Enable writing to color buffer
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
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
