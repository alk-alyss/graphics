#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "model.hpp"
#include "player.hpp"

class Portal: public Model {
private:
    std::shared_ptr<Portal> linkedPortal;
    Camera camera;

public:
    Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size);

    std::shared_ptr<Portal> getLinkedPortal() const {return linkedPortal;}

    void linkPortal(std::shared_ptr<Portal> portal);
    void handleCollision(std::shared_ptr<Player> player);

    glm::vec4 getClipPlane() const;

    bool linePlaneIntersection(const glm::vec3 a, const glm::vec3 b);

    void getViewFromLinkedPortal(
        const glm::vec3 position,
        const glm::quat orientation,
        glm::vec3& newPosition,
        glm::quat& newOrientation
    );

    void updateCamera(const Camera& playerCamera);
    Camera getCamera() const { return camera;};
};

#endif
