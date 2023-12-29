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
#define MAX_POINT_LIGHTS 0

class Renderer {
private:
    Shader shader;
    GLuint matricesUBO, lightsUBO;

private:
    void uploadMatrices(const Camera& camera);
    void uploadLights(
        const std::vector<DirectionalLight> directionalLights,
        const std::vector<PointLight> pointLights
    );

public:
    Renderer(const Shader& shader);

    void render(
            const Camera& camera,
            const Scene& scene
        );
};

#endif
