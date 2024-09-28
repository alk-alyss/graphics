#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "material.hpp"
#include "orientable.hpp"

class Model : public Orientable {
private:
    const std::shared_ptr<Mesh> mesh;
    const Material material;

public:
    Model(
        const std::shared_ptr<Mesh> mesh,
        const Material material
    ) : mesh(mesh), material(material) {}

    const AABB getAABB() const;

    virtual void draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const;

    glm::vec3 getClosestBlockNormal(const glm::vec3 position) const;
};

#endif
