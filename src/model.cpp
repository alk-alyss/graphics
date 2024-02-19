#include "model.hpp"

Model::Model(
    const std::shared_ptr<Mesh> mesh,
    const Material material,
    const Transformation meshTransformation
) : mesh(mesh), meshTransformation(meshTransformation), material(material) {
    std::shared_ptr<Node> transformedMesh = std::make_shared<Transformation>(meshTransformation);
    transformedMesh->children.push_back(mesh);

    std::shared_ptr<Node> materialPtr = std::make_shared<Material>(material);
    materialPtr->children.push_back(transformedMesh);

    this->children.push_back(materialPtr);
}

void Model::draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const {
    modelMatrix = modelMatrix * this->modelMatrix();

    Node::draw(modelMatrix, shader);
}
