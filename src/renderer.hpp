#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Renderer {
private:
    Shader shader;
    GLuint matricesUBO;

private:
    void uploadMatrices(const Camera& camera);

public:
    Renderer(Shader& shader);

    void render(const Camera& camera, const std::vector<std::shared_ptr<Mesh>> meshList);
};

#endif
