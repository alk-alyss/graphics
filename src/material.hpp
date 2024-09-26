#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SOIL2.h>

#include "common.hpp"

struct Texture {
    GLuint textureId = 0;
    std::string path = "";

    Texture() = delete;
    Texture(const std::string imagePath, const bool sRGB);
    ~Texture();

    //Delete the copy constructor/assignment.
    Texture(const Texture&) = delete;
    Texture &operator=(const Texture&) = delete;

    Texture(Texture&& other) :
        textureId(other.textureId),
        path(other.path)
    {
        other.textureId = 0; //Use the "null" texture for the old object.
    }

    Texture &operator=(Texture&& other) {
        //ALWAYS check for self-assignment.
        if(this != &other)
        {
            releaseTexture();
            std::swap(textureId, other.textureId);
            std::swap(path, other.path);
        }

        return *this;
    }

    void releaseTexture();

    void bind() const;

    static void bindDefault();
};

struct Material : Node{
    std::shared_ptr<Texture> albedo = nullptr;
    std::shared_ptr<Texture> ao = nullptr;
    std::shared_ptr<Texture> height = nullptr;
    std::shared_ptr<Texture> metallic = nullptr;
    std::shared_ptr<Texture> normal = nullptr;
    std::shared_ptr<Texture> roughness = nullptr;

    Material() = default;

    Material(
        const std::shared_ptr<Texture> albedo,
        const std::shared_ptr<Texture> ao,
        const std::shared_ptr<Texture> height,
        const std::shared_ptr<Texture> metallic,
        const std::shared_ptr<Texture> normal,
        const std::shared_ptr<Texture> roughness
    ) : albedo(albedo),
        ao(ao),
        height(height),
        metallic(metallic),
        normal(normal),
        roughness(roughness)
    {};

    Material(const std::string materialPath);

    void bind() const;
    void draw(const glm::mat4 modelMatrix, const std::shared_ptr<Shader> shader) const override;
};

#endif
