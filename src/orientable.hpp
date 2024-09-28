#ifndef ORIENTABLE_HPP
#define ORIENTABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define DEFAULT_POSITION glm::vec3(0)
#define DEFAULT_ORIENTATION glm::quat(1, 0, 0, 0)
#define DEFAULT_SCALE glm::vec3(1)

glm::quat eulerToQuat(const float pitch, const float yaw, const float roll);

class Orientable {
protected:
    glm::vec3 position = DEFAULT_POSITION;
    glm::quat orientation = DEFAULT_ORIENTATION;
    glm::vec3 scale = DEFAULT_SCALE;

public:
    glm::vec3 up() const;
    glm::vec3 forward() const;
    glm::vec3 right() const;

public:
    Orientable() : Orientable(DEFAULT_POSITION) {};
    Orientable(const glm::vec3 position) : Orientable(position, DEFAULT_ORIENTATION) {};
    Orientable(const glm::vec3 position, const glm::quat orientation) : Orientable(position, orientation, DEFAULT_SCALE) {};
    Orientable(const glm::vec3 position, const glm::quat orientation, const glm::vec3 scale);

    // Setters
    virtual void setPosition(const glm::vec3 position);
    void setPosition(const float x, const float y, const float z) {setPosition(glm::vec3(x, y, z));}
    virtual void setOrientation(const glm::quat orientation);
    void setOrientation(const float pitch, const float yaw, const float roll) {setOrientation(eulerToQuat(pitch, yaw, roll));}
    virtual void setScale(const glm::vec3 newScale);
    void setScale(const float newScale) {setScale(glm::vec3(newScale, newScale, newScale));}

    // Getters
    glm::vec3 getPosition() const {return position;}
    glm::quat getOrientation() const {return orientation;}
    glm::vec3 getScale() const {return scale;}

    // Matrices
    glm::mat4 translationMatrix() const;
    glm::mat4 rotationMatrix() const;
    glm::mat4 scallingMatrix() const;
    glm::mat4 modelMatrix() const;

    // Transformations
    virtual void translate(const glm::vec3 translation);
    void translate(const float x, const float y, const float z) {translate(glm::vec3(x, y, z));}

    virtual void changeScale(const glm::vec3 scaleFactor);
    void changeScale(const float scaleFactor) {changeScale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));}

    virtual void rotate(const glm::quat rotation);
    void rotate(const float pitch, const float yaw, const float roll) {rotate(eulerToQuat(pitch, yaw, roll));}
    virtual void rotate(const float angle, const glm::vec3 axis);

    virtual void lookAt(const glm::vec3 target, const glm::vec3 up = glm::vec3(0, 1, 0), const glm::vec3 alternativeUp = glm::vec3(0,0,1));
};

#endif
