#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "orientable.hpp"
#include "shader.hpp"

struct Leaf {
    virtual void draw(glm::mat4 modelMatrix, Shader& shader) const = 0;
};

struct Node : Leaf {
    std::vector<std::shared_ptr<Node>> children;

    virtual void draw(glm::mat4 modelMatrix, Shader& shader) const {
        for (auto& child : children) {
            child->draw(modelMatrix, shader);
        }
    }
};

struct Transformation : private Orientable, public Node {
    void draw(glm::mat4 modelMatrix, Shader& shader) const override {
        modelMatrix = modelMatrix * this->modelMatrix();

        Node::draw(modelMatrix, shader);
    }
};


#endif
