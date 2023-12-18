#ifndef CUBE_HPP
#define CUBE_HPP

#include "mesh.hpp"
#include "material.hpp"
#include "common.hpp"

class Cube : public Mesh {
private:
    Material material;
public:
    Cube(Material material) : material(material), Mesh("resources/models/cube.obj") {}
};

#endif
