#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "model.hpp"
#include "player.hpp"

class Portal: public Model {
private:
    std::shared_ptr<Portal> linkedPortal;
    std::unique_ptr<Camera> camera;

    bool linePlaneIntersection(const glm::vec3 a, const glm::vec3 b) const;

public:
    Portal(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 size, const Material material);

    std::shared_ptr<Portal> getLinkedPortal() const {return linkedPortal;}

    void linkPortal(const std::shared_ptr<Portal> portal);
    void handleCollision(const std::shared_ptr<Player> player);

    glm::vec4 getPlane() const;

    void getViewFromLinkedPortal(
        const glm::vec3 position,
        const glm::quat orientation,
        glm::vec3& newPosition,
        glm::quat& newOrientation
    ) const;

    void updateCamera(const Camera& playerCamera);
    const Camera& getCamera() const {return *camera;}
};

#endif
