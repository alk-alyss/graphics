#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>

class Orientable {
protected:
    glm::vec3 position;

    glm::vec3 direction;
    glm::vec3 right;

protected:
    glm::vec3 up();

public:
    void setPosition(float x, float y, float z) {this->position = glm::vec3(x, y, z);}
    void setPosition(glm::vec3 position) {this->position = position;}

    void setRotation(glm::vec3 rotation);
    void setRotation(float pitch, float yaw, float roll) {setRotation(glm::vec3(pitch, yaw, roll));}

    glm::vec3 getPosition() {return this->position;}
    glm::vec3 getDirection() {return this->direction;}
    glm::vec3 getRight() {return this->right;}

    glm::vec3 getRotation();

    void translate(glm::vec3 translation);
    void translate(float x, float y, float z) {translate(glm::vec3(x, y, z));}

    void rotate(glm::vec3 rotation);
    void rotate(float pitch, float yaw, float roll) {rotate(glm::vec3(pitch, yaw, roll));}
    void rotate(float pitch, float yaw) {rotate(pitch, yaw, 0.0f);}
    void rotate(float angle, glm::vec3 axis);

    void lookAt(glm::vec3 target, glm::vec3 up);
};

#endif
