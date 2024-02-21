#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include "scene.hpp"

void checkCollisions(const Scene& scene, const float deltaTime);
std::shared_ptr<Model> castRay(const Scene& scene, const glm::vec3 position, const glm::vec3 direction, glm::vec3& normalVector);

#endif
