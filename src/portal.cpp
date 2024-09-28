#include "portal.hpp"
#include "glm/matrix.hpp"
#include "resources.hpp"
#include "glm/gtc/matrix_access.hpp"

Portal::Portal(
    const glm::vec3 position,
    const glm::vec3 direction,
    const glm::vec3 size,
    const Material material
) : Model(planeMesh, material, Transformation(glm::vec3(0), glm::vec3(glm::radians(-90.0f), 0, 0))) {
    setPosition(position);
    setScale(size);
    lookAt(position+direction);

    camera = std::make_unique<Camera>();
    camera->setPosition(position);
    camera->lookAt(position+direction);
}

void Portal::linkPortal(const std::shared_ptr<Portal> portal) {
    linkedPortal = portal;
}

bool Portal::linePlaneIntersection(
    const glm::vec3 a,
    const glm::vec3 b
) const {
    glm::vec3 normal = forward();
    glm::vec3 position = getPosition();

    // Translate points to portal space
    glm::vec3 portalA = a - position;
    glm::vec3 portalB = b - position;

    // Find which side of the plane the points are on
    float AdotN = glm::dot(portalA, normal);
    float BdotN = glm::dot(portalB, normal);

    // If both points are on the same side of the plane (sign(AdotN) = sign(BdotN)), no intersection
    return !(glm::sign(AdotN) == glm::sign(BdotN));
}

void Portal::handleCollision(const std::shared_ptr<Player> player) {
    if (linkedPortal == nullptr) return;

    glm::vec3 prevPosition = player->getPreviousPosition();
    glm::vec3 position = player->getPosition();
    if (position == prevPosition) return;

    if (!linePlaneIntersection(prevPosition, position)) return;

    glm::vec3 newPosition;
    glm::quat newOrientation;
    getViewFromLinkedPortal(player->getCamera().getPosition(), player->getCamera().getOrientation(), newPosition, newOrientation);

    player->setPosition(newPosition);
    player->setOrientation(newOrientation);
    player->setVelocity(0);
}

void Portal::getViewFromLinkedPortal(
    const glm::vec3 position,
    const glm::quat orientation,
    glm::vec3& newPosition,
    glm::quat& newOrientation
) const {
    if (linkedPortal == nullptr) return;

    // Transformmation from world space to this portal's local space
    glm::mat4 world2src = glm::inverse(modelMatrix());
    // Rotate 180 degrees around the y-axis
    glm::mat4 rotate180 = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
    // Transformation from the linked portal's space to world space
    glm::mat4 dest2world = linkedPortal->modelMatrix();

    glm::mat4 transformation = dest2world * rotate180 * world2src;

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

glm::vec4 Portal::getPlane() const {
    glm::vec3 normal = forward();
    glm::vec3 position = getPosition();

    // Plane equation: ax + by + cz + d = 0, where (a, b, c) is the normal vector, and d = -dot(normal, position)
    return glm::vec4(normal, -glm::dot(normal, position));
}

void Portal::updateCamera(const Camera& playerCamera) {
    if (linkedPortal == nullptr) return;

    glm::vec3 cameraPosition;
    glm::quat cameraOrientation;

    getViewFromLinkedPortal(playerCamera.getPosition(), playerCamera.getOrientation(), cameraPosition, cameraOrientation);

    camera->setPosition(cameraPosition);
    camera->setOrientation(cameraOrientation);

    // Get linked portal plane in camera space
    glm::vec4 clipPlane = glm::inverse(glm::transpose(camera->getView())) * linkedPortal->getPlane();

    glm::mat4 projectionMatrix = playerCamera.getProjection();
    projectionMatrix = obliqueFrustum(projectionMatrix, clipPlane);

    camera->setProjection(projectionMatrix);
}

void Portal::draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const {
    Model::draw(modelMatrix, shader);
}
