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
    glm::mat4 initialTransformation{1};

    std::shared_ptr<Shader> singleShader, instancedShader, simpleShader;
    GLuint matricesUBO, lightsUBO;
    size_t lightsUBOsize;

private:
    void uploadMatrices(const Camera& camera);
public:
    Renderer();

    void uploadLights(const Scene& scene);

    void render(const Scene& scene);
    void renderScene(const Scene& scene, const Camera& camera);
    void renderPortals(const Scene& scene);
};

#endif
