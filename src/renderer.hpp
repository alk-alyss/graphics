#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include <glm/glm.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "scene.hpp"

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 1

class Renderer {
private:
    std::shared_ptr<Shader> singleShader, instancedShader;
    GLuint matricesUBO, lightsUBO;
    size_t lightsUBOsize;

private:
    void uploadMatrices(const Camera& camera);
public:
    Renderer(std::shared_ptr<Shader> singleShader, std::shared_ptr<Shader> instancedShader);

    void uploadLights(const Scene& scene);

    void render(const Scene& scene);
};

#endif
