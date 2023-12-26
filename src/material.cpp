#include "material.hpp"

#include <iostream>

using namespace std;

Texture::Texture(const string imagePath) {
    cout << "Reading image: " << imagePath << endl;

    path = imagePath;

    //Load Image File Directly into an OpenGL Texture
    textureId = SOIL_load_OGL_texture
    (
        imagePath.c_str(),
        SOIL_LOAD_RGB,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
    );

    // error check
    if (textureId == 0) {
        cout << "SOIL loading error: " << SOIL_last_result() << endl;
    }
}

Texture::~Texture() {
   glDeleteTextures(1, &textureId);
}

void Texture::bind() const{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::bindDefault() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


Material::Material(const std::string materialPath) {
    // TODO: load material textures from directory
}

void Material::load() const {
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
}
