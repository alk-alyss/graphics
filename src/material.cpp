#include "material.hpp"

#include <iostream>
#include <filesystem>

Texture::Texture(const std::string imagePath, bool sRGB = false) {
    std::cout << "Reading image: " << imagePath << std::endl;

    path = imagePath;

    uint flags = SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS;

    if (sRGB) flags |= SOIL_FLAG_SRGB_COLOR_SPACE;

    //Load Image File Directly into an OpenGL Texture
    textureId = SOIL_load_OGL_texture
    (
        imagePath.c_str(),
        SOIL_LOAD_RGB,
        SOIL_CREATE_NEW_ID,
        flags
    );

    // error check
    if (textureId == 0) {
        std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
    }
}

Texture::~Texture() {
    releaseTexture();
}

void Texture::releaseTexture() {
    glDeleteTextures(1, &textureId);
}

void Texture::bind() const{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::bindDefault() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


Material::Material(const std::string materialPath) {
    const std::filesystem::path directory(materialPath);

    for (auto const& dir_entry : std::filesystem::directory_iterator(directory)) {
        if (!dir_entry.is_regular_file()) continue;

        std::string filepath = dir_entry.path();

        if (
            filepath.find("albedo") != std::string::npos ||
            filepath.find("Color") != std::string::npos
        ) {
            albedo = std::make_shared<Texture>(filepath, true);
        }
        else if (
            filepath.find("ao") != std::string::npos ||
            filepath.find("AmbientOcclusion") != std::string::npos
        ) {
            ao = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("height") != std::string::npos ||
            filepath.find("Displacement") != std::string::npos
        ) {
            height = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("metallic") != std::string::npos ||
            filepath.find("metalness") != std::string::npos ||
            filepath.find("Metallic") != std::string::npos
        ) {
            metallic = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("normal") != std::string::npos ||
            filepath.find("NormalGL") != std::string::npos
        ) {
            normal = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("roughness") != std::string::npos ||
            filepath.find("Roughness") != std::string::npos
        ) {
            roughness = std::make_shared<Texture>(filepath);
        }
    }
}

void Material::draw(glm::mat4 modelMatrix, Shader& shader) const {
    glActiveTexture(GL_TEXTURE0);
    if (albedo.get() != nullptr) {
        albedo->bind();
    } else {
        Texture::bindDefault();
    }

    glActiveTexture(GL_TEXTURE1);
    if (ao.get() != nullptr) {
        ao->bind();
    } else {
        Texture::bindDefault();
    }

    glActiveTexture(GL_TEXTURE2);
    if (height.get() != nullptr) {
        height->bind();
    } else {
        Texture::bindDefault();
    }

    glActiveTexture(GL_TEXTURE3);
    if (metallic.get() != nullptr) {
        metallic->bind();
    } else {
        Texture::bindDefault();
    }

    glActiveTexture(GL_TEXTURE4);
    if (normal.get() != nullptr) {
        normal->bind();
    } else {
        Texture::bindDefault();
    }

    glActiveTexture(GL_TEXTURE5);
    if (roughness.get() != nullptr) {
        roughness->bind();
    } else {
        Texture::bindDefault();
    }

    Node::draw(modelMatrix, shader);
}
