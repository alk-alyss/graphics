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

#include <iostream>
#include <glm/gtx/string_cast.hpp>

AABB Model::getAABB() const {
    const AABB meshAABB = mesh->getAABB();

    glm::vec4 AABBmin(meshAABB.min, 1);
    glm::vec4 AABBmax(meshAABB.max, 1);

    glm::vec4 transformedMin = modelMatrix() * meshTransformation.getMatrix() * AABBmin;
    glm::vec4 transformedMax = modelMatrix() * meshTransformation.getMatrix() * AABBmax;

    std::vector<glm::vec3> vertices {
        glm::vec3(transformedMin.x, transformedMin.y, transformedMin.z),
        glm::vec3(transformedMin.x, transformedMin.y, transformedMax.z),
        glm::vec3(transformedMin.x, transformedMax.y, transformedMin.z),
        glm::vec3(transformedMin.x, transformedMax.y, transformedMax.z),
        glm::vec3(transformedMax.x, transformedMin.y, transformedMin.z),
        glm::vec3(transformedMax.x, transformedMin.y, transformedMax.z),
        glm::vec3(transformedMax.x, transformedMax.y, transformedMin.z),
        glm::vec3(transformedMax.x, transformedMax.y, transformedMax.z)
    };

    const AABB modelAABB = AABB(vertices);

    return modelAABB;
}

glm::vec3 Model::getClosestBlockNormal(const glm::vec3 point) const {
    glm::vec3 modelToPoint = glm::normalize(point - getPosition());
    glm::vec3 normalVector = modelToPoint;

    float xDot = glm::dot(modelToPoint, glm::vec3(1, 0, 0));
    float zDot = glm::dot(modelToPoint, glm::vec3(0, 0, 1));
    /* std::cout << "xDot: " << xDot << " zDot: " << zDot << " "; */

    float limit = 0.5;
    if (std::abs(xDot) > std::abs(zDot)) {
        if (xDot > limit) {
            normalVector = glm::vec3(1,0,0);
        } else if (xDot < -limit) {
            normalVector = glm::vec3(-1,0,0);
        }
    } else {
        if (zDot > limit) {
            normalVector = glm::vec3(0,0,1);
        } else if (zDot < -limit) {
            normalVector = glm::vec3(0,0,-1);
        }
    }

    return normalVector;
}


void Model::draw(glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const {
    modelMatrix = modelMatrix * this->modelMatrix();

    Node::draw(modelMatrix, shader);
}
