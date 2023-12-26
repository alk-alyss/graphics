#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <memory>

#include "mesh.hpp"
#include "material.hpp"

struct Model : public Orientable, public Node {
    Model(
        std::shared_ptr<Node> mesh,
        std::shared_ptr<Material> material,
        glm::vec3 position,
        glm::vec3 rotation
    );

    void draw(glm::mat4 modelMatrix, Shader& shader) const;
};

#endif
