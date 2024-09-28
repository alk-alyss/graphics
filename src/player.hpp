#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "model.hpp"
#include "orientable.hpp"
#include "camera.hpp"

class Player : public Orientable {
    std::unique_ptr<Camera> camera;
    std::shared_ptr<Model> model;

    bool noClip = false;

    float movementSpeed = 10.0f;
    float mouseSpeed = 0.1f;

    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 forwardMovementVector();
    void updateComponents();

    glm::vec3 previousPosition = glm::vec3(0);

public:
    Player() = delete;
    Player(const std::shared_ptr<Model> model) : Player(model, DEFAULT_POSITION, DEFAULT_ORIENTATION, DEFAULT_SCALE) {}
    Player(const std::shared_ptr<Model> model, const glm::vec3 position) : Player(model, position, DEFAULT_ORIENTATION, DEFAULT_SCALE) {}
    Player(const std::shared_ptr<Model> model, const glm::vec3 position, const glm::quat orientation) : Player(model, position, orientation, DEFAULT_SCALE) {}
    Player(const std::shared_ptr<Model> model, const glm::vec3 position, const glm::quat orientation, const glm::vec3 scale);

    void update(const float deltaTime);

    void updateCamera(GLFWwindow* window) {camera->updateAspectRatio(window);}
    const Camera& getCamera() {return *camera;}

    void setMovementSpeed(const float movementSpeed) {this->movementSpeed = movementSpeed;}
    void setMouseSpeed(const float mouseSpeed) {this->mouseSpeed = mouseSpeed;}

    void setPosition(const glm::vec3 position) override;
    void setOrientation(const glm::quat orientation) override;

    void setVelocity(const float velocity) {this->velocity = forwardMovementVector() * velocity;}
    void setVelocity(const glm::vec3 velocity) {this->velocity = velocity;}
    glm::vec3 getVelocity() const {return velocity;}

    void translate(const glm::vec3 translation) override;
    void rotate(const glm::quat rotation) override;

    void look(const float mouseX, const float mouseY, const float deltaTime);
    void lookAt(const glm::vec3 target, const glm::vec3 up = glm::vec3(0, 1, 0), const glm::vec3 alternativeUp = glm::vec3(0,0,1)) override;

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void zoom(const float amount) {camera->zoom(amount);}

    bool isNoClip() const {return noClip;}
    void toggleNoClip();

    AABB getCollider() const {return model->getAABB();}

    glm::vec3 getPreviousPosition() const {return previousPosition;}

    void draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const;
};

#endif
