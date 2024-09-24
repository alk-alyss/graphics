#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "model.hpp"
#include "orientable.hpp"
#include "camera.hpp"

class Player : public Orientable, Node {
    std::unique_ptr<Camera> camera;
    std::shared_ptr<Model> model;
    std::shared_ptr<Model> collider;

    bool noClip = false;

    float movementSpeed = 10.0f;
    float mouseSpeed = 0.1f;

    glm::vec3 velocity;
    glm::vec3 forwardMovementVector();
    void updateComponents();

    glm::vec3 previousPosition;

public:
    Player(
        std::shared_ptr<Model> model,
        std::shared_ptr<Model> collider,
        glm::vec3 position = DEFAULT_POSITION,
        glm::quat orientation = DEFAULT_ORIENTATION,
        glm::vec3 scale = DEFAULT_SCALE
    );

    void update(float deltaTime);

    void updateCamera(GLFWwindow* window) {camera->updateAspectRatio(window);}
    const Camera& getCamera() {return *camera;}

    void setMovementSpeed(float movementSpeed) {this->movementSpeed = movementSpeed;}
    void setMouseSpeed(float mouseSpeed) {this->mouseSpeed = mouseSpeed;}

    void setPosition(glm::vec3 position) override;
    void setOrientation(glm::quat orientation) override;

    void setVelocity(glm::vec3 velocity) {this->velocity = velocity;}
    glm::vec3 getVelocity() const {return velocity;}

    void translate(glm::vec3 translation) override;
    void rotate(glm::quat rotation) override;

    void look(float mouseX, float mouseY, float deltaTime);
    void lookAt(const glm::vec3 target, const glm::vec3 up = glm::vec3(0, 1, 0), const glm::vec3 alternativeUp = glm::vec3(0,0,1)) override;

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void zoom(float amount) {camera->zoom(amount);}

    bool isNoClip() {return noClip;}
    void toggleNoClip();

    AABB getCollider() {return collider->getAABB();}

    glm::vec3 getPreviousPosition() {return previousPosition;}

    void draw(glm::mat4 modelMatrix, std::shared_ptr<Shader> shader) const override;
};

#endif
