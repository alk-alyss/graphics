#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <memory>

#include "common.hpp"
#include "mesh.hpp"
#include "material.hpp"

class Model : public Orientable, public Node {
private:
    const std::shared_ptr<Mesh> mesh;
    const Transformation meshTransformation;
    const Material material;

public:
    Model(
        std::shared_ptr<Mesh> mesh,
        Material material,
        Transformation meshTransformation = Transformation()
    );

    AABB getAABB() const;

    void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const override;
};

#endif
