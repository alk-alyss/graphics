#include "model.hpp"

Model::Model(
    std::shared_ptr<Node> mesh,
    Material material,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
) : Orientable(position, rotation, scale) {
    std::shared_ptr<Node> materialPtr = std::make_shared<Material>(material);
    materialPtr->children.push_back(mesh);
    this->children.push_back(materialPtr);
}

void Model::draw(glm::mat4 modelMatrix, Shader& shader) const {
    modelMatrix = modelMatrix * this->modelMatrix();

    Node::draw(modelMatrix, shader);
}
