#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define DEFAULT_POSITION glm::vec3(0)
#define DEFAULT_ORIENTATION glm::vec3(0)

class Orientable {
protected:
    glm::vec3 position;
    glm::quat orientation;

protected:
    glm::vec3 up() const;
    glm::vec3 forward() const;
    glm::vec3 right() const;

public:
    Orientable(glm::vec3 position = DEFAULT_POSITION, glm::vec3 rotation = DEFAULT_ORIENTATION);

    void setPosition(float x, float y, float z) {this->position = glm::vec3(x, y, z);}
    void setPosition(glm::vec3 position) {this->position = position;}

    void setRotation(glm::vec3 rotation);
    void setRotation(float pitch, float yaw, float roll) {setRotation(glm::vec3(pitch, yaw, roll));}

    glm::mat4 getRotationMatrix() const;

    void translate(glm::vec3 translation);
    void translate(float x, float y, float z) {translate(glm::vec3(x, y, z));}

    void rotate(glm::vec3 rotation);
    void rotate(float pitch, float yaw, float roll) {rotate(glm::vec3(pitch, yaw, roll));}
    void rotate(float pitch, float yaw);
    void rotate(float angle, glm::vec3 axis);

    void lookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
