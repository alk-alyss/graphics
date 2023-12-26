#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SOIL2.h>

struct Texture {
    GLuint textureId = 0;
    std::string path;

    Texture(const std::string imagePath);
    ~Texture();

    void bind() const;
};

struct Material {
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<Texture> ao;
    std::shared_ptr<Texture> height;
    std::shared_ptr<Texture> metallic;
    std::shared_ptr<Texture> normal;
    std::shared_ptr<Texture> roughness;

    void load() const;
};

#endif
