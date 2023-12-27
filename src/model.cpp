#include "model.hpp"

Model::Model(
    std::shared_ptr<Node> mesh,
    std::shared_ptr<Material> material,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
) : Orientable(position, rotation, scale) {
    material->children.push_back(mesh);
    this->children.push_back(material);
}

void Model::draw(glm::mat4 modelMatrix, Shader& shader) const {
    modelMatrix = modelMatrix * this->modelMatrix();

    Node::draw(modelMatrix, shader);
}
