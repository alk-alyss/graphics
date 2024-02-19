#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "orientable.hpp"
#include "shader.hpp"

struct Node {
    std::vector<std::shared_ptr<Node>> children;

    virtual ~Node() = default;

    virtual void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const {
        for (auto& child : children) {
            child->draw(modelMatrix, shader);
        }
    }
};

struct Transformation : private Orientable, public Node {
    using Orientable::Orientable;

    glm::mat4 getMatrix() const {return modelMatrix();}

    void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const override {
        modelMatrix = modelMatrix * this->modelMatrix();

        Node::draw(modelMatrix, shader);
    }
};


#endif
