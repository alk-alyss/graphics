#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <mesh.hpp>
#include <shader.hpp>

class Renderer {
    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        
        glm::mat4 VP;

        Shader& shader;
        GLuint MVPLocation;

    public:
        Renderer(Shader& shader);

        void render(std::vector<std::shared_ptr<Mesh>> meshList);
};

#endif