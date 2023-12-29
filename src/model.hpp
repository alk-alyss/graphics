#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <memory>

#include "common.hpp"
#include "mesh.hpp"
#include "material.hpp"

struct Model : public Orientable, public Node {
    Model(
        std::shared_ptr<Node> mesh,
        Material material,
        glm::vec3 position = DEFAULT_POSITION,
        glm::vec3 rotation = DEFAULT_ORIENTATION,
        glm::vec3 scale = DEFAULT_SCALE
    );

    void draw(glm::mat4 modelMatrix, Shader& shader) const;
};

#endif
