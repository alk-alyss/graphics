#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <mesh.hpp>
#include <shader.hpp>
#include <camera.hpp>

class Renderer {
private:
    Shader& shader;
    GLuint MVPLocation;

public:
    Renderer(Shader& shader);

    void render(Camera& camera, std::vector<std::shared_ptr<Mesh>> meshList);
};

#endif