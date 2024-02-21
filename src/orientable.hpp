#ifndef ORIENTABLE_HPP
#define ORIENTABLE_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define DEFAULT_POSITION glm::vec3(0)
#define DEFAULT_ORIENTATION glm::vec3(0)
#define DEFAULT_SCALE glm::vec3(1)

class Orientable {
protected:
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

public:
    glm::vec3 up() const;
    glm::vec3 forward() const;
    glm::vec3 right() const;

public:
    Orientable(
        glm::vec3 position = DEFAULT_POSITION,
        glm::vec3 rotation = DEFAULT_ORIENTATION,
        glm::vec3 scale = DEFAULT_SCALE
    );

    virtual void setPosition(glm::vec3 position) {this->position = position;}
    void setPosition(float x, float y, float z) {setPosition(glm::vec3(x, y, z));}

    virtual void setRotation(glm::vec3 rotation);
    void setRotation(float pitch, float yaw, float roll) {setRotation(glm::vec3(pitch, yaw, roll));}

    virtual void setScale(glm::vec3 newScale) {this->scale = newScale;}
    void setScale(float newScale) {setScale(glm::vec3(newScale, newScale, newScale));}

    glm::vec3 getPosition() const {return position;}
    glm::vec3 getSize() const {return scale;}

    glm::mat4 translationMatrix() const;
    glm::mat4 rotationMatrix() const;
    glm::mat4 scallingMatrix() const;
    glm::mat4 modelMatrix() const;

    virtual void translate(glm::vec3 translation);
    void translate(float x, float y, float z) {translate(glm::vec3(x, y, z));}

    virtual void changeScale(glm::vec3 scaleFactor);
    void changeScale(float scaleFactor) {changeScale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));}

    virtual void rotate(glm::vec3 rotation);
    void rotate(float pitch, float yaw, float roll) {rotate(glm::vec3(pitch, yaw, roll));}
    virtual void rotate(float angle, glm::vec3 axis);

    virtual void lookAt(const glm::vec3 target, const glm::vec3 up = glm::vec3(0, 1, 0), const glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
