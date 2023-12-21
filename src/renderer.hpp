#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"

class Renderer {
private:
    Shader shader;
    GLuint matricesUBO, lightsUBO, materialUBO;

private:
    void uploadMatrices(const Camera& camera);
    void uploadLights(const Light& light);
    void uploadMaterial(const Material& mtl);

public:
    Renderer(Shader& shader);

    void render(
            const Camera& camera,
            const std::vector<std::shared_ptr<Mesh>>& meshList,
            const Light& light,
            const Material& material
        );
};

#endif
