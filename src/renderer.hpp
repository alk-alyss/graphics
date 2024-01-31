#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "scene.hpp"

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 1

class Renderer {
private:
    std::shared_ptr<Shader> shader;
    GLuint matricesUBO, lightsUBO;
    size_t lightsUBOsize;

private:
    void uploadMatrices(const Camera& camera);
    void uploadLights(
        const std::vector<DirectionalLight> directionalLights,
        const std::vector<PointLight> pointLights
    );

public:
    Renderer(std::shared_ptr<Shader> shader);

    void render(const Scene& scene);
};

#endif
