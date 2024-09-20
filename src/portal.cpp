#include "portal.hpp"
#include "glm/matrix.hpp"
#include "resources.hpp"
#include "glm/gtc/matrix_access.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

Portal::Portal(
    const glm::vec3 position,
    const glm::vec3 direction,
    const glm::vec3 size
) : Model(planeMesh, metalMaterial, Transformation(glm::vec3(0), glm::vec3(glm::radians(-90.0f), 0, 0))) {
    setPosition(position);
    setScale(size);
    lookAt(position+direction);

    camera.setPosition(position);
    camera.lookAt(position+direction);
}

void Portal::linkPortal(std::shared_ptr<Portal> portal) {
    linkedPortal = portal;
}

glm::vec4 Portal::getClipPlane() const {
    glm::vec3 normal = forward();
    glm::vec3 position = getPosition();

    glm::vec4 plane = glm::vec4(normal, -glm::dot(normal, position));
    plane = glm::inverse(glm::transpose(camera.getView())) * plane;

    return plane;
}

void Portal::handleCollision(std::shared_ptr<Player> player) {
    if (linkedPortal == nullptr) return;

    glm::vec3 portalNormal = linkedPortal->forward();
    glm::vec3 newPosition = linkedPortal->getPosition() + portalNormal;

    glm::vec3 playerForward = player->forward();
    glm::vec3 lookAt = newPosition + portalNormal;;

    std::cout << glm::to_string(portalNormal) << " " << glm::to_string(newPosition) << " " << glm::to_string(lookAt) << std::endl;

    player->translate(newPosition - player->getPosition());
    player->lookAt(lookAt);
}

void Portal::getViewFromLinkedPortal(
    const glm::vec3 position,
    const glm::quat orientation,
    glm::vec3& newPosition,
    glm::quat& newOrientation
) {
    if (linkedPortal == nullptr) return;

    glm::mat4 world2dest = glm::inverse(linkedPortal->modelMatrix());
    glm::mat4 rotate180 = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
    glm::mat4 src2world = modelMatrix();

    glm::mat4 transformation = src2world * rotate180 * world2dest;

    newPosition = transformation * glm::vec4(position, 1);
    newOrientation = transformation * glm::mat4_cast(orientation);
}

// "Modifying the Projection Matrix to Perform Oblique Near-Plane Clipping" by Eric Lengyel
// https://terathon.com/blog/oblique-clipping.html
// "Oblique View Frustum Depth Projection and Clipping" by Eric Lengyel
// https://terathon.com/lengyel/Lengyel-Oblique.pdf
glm::mat4 obliqueFrustum(const glm::mat4& projectionMatrix, const glm::vec4& clipPlane) {
    if (clipPlane.w > 0.0f) return projectionMatrix;

    glm::mat4 newProj = projectionMatrix;

    glm::vec4 q (
        (glm::sign(clipPlane.x) + newProj[2][0]) / newProj[0][0],
        (glm::sign(clipPlane.y) + newProj[2][1]) / newProj[1][1],
        -1.0f,
        (1.0f + newProj[2][2]) / newProj[3][2]
    );

    glm::vec4 c = clipPlane * (2.0f / glm::dot(q, clipPlane));

    glm::vec4 newRow = c - glm::row(newProj, 3);
    newProj = glm::row(newProj, 2, newRow);

    return newProj;
}

void Portal::updateCamera(const Camera& playerCamera) {
    if (linkedPortal == nullptr) return;

    glm::vec3 cameraPosition;
    glm::quat cameraOrientation;

    getViewFromLinkedPortal(playerCamera.getPosition(), playerCamera.getOrientation(), cameraPosition, cameraOrientation);

    camera.setPosition(cameraPosition);
    camera.setOrientation(cameraOrientation);

    glm::mat4 projectionMatrix = playerCamera.getProjection();
    glm::vec4 clipPlane = getClipPlane();

    projectionMatrix = obliqueFrustum(projectionMatrix, clipPlane);

    camera.setProjection(projectionMatrix);
}
