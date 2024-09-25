#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "material.hpp"
#include "mesh.hpp"

inline std::shared_ptr<Mesh> planeMesh;
inline std::shared_ptr<Mesh> sphereMesh;
inline std::shared_ptr<Mesh> cubeMesh;
inline std::shared_ptr<Mesh> suzanneMesh;

inline Material grassMaterial;
inline Material dirtMaterial;
inline Material metalMaterial;
inline Material scuffedPlasticMaterial;

#endif
