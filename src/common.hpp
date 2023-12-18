#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Orientable {
protected:
    glm::vec3 position = glm::vec3(0);

    glm::quat orientation = glm::quat(glm::vec3(0,0,0));

protected:
    glm::vec3 up();
    glm::vec3 forward();
    glm::vec3 right();
    glm::vec3 direction() {return forward();}

public:
    void setPosition(float x, float y, float z) {this->position = glm::vec3(x, y, z);}
    void setPosition(glm::vec3 position) {this->position = position;}

    void setRotation(glm::vec3 rotation);
    void setRotation(float pitch, float yaw, float roll) {setRotation(glm::vec3(pitch, yaw, roll));}

    glm::mat4 getRotationMatrix();

    void translate(glm::vec3 translation);
    void translate(float x, float y, float z) {translate(glm::vec3(x, y, z));}

    void rotate(glm::vec3 rotation);
    void rotate(float pitch, float yaw, float roll) {rotate(glm::vec3(pitch, yaw, roll));}
    void rotate(float pitch, float yaw) {rotate(pitch, yaw, 0.0f);}
    void rotate(float angle, glm::vec3 axis);

    void lookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
