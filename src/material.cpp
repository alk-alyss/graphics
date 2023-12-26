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

void Material::load() const {
    if (albedo.get() != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();
    }

    if (ao.get() != nullptr) {
        glActiveTexture(GL_TEXTURE1);
        ao->bind();
    }

    if (height.get() != nullptr) {
        glActiveTexture(GL_TEXTURE2);
        height->bind();
    }

    if (metallic.get() != nullptr) {
        glActiveTexture(GL_TEXTURE3);
        metallic->bind();
    }

    if (normal.get() != nullptr) {
        glActiveTexture(GL_TEXTURE4);
        normal->bind();
    }

    if (roughness.get() != nullptr) {
        glActiveTexture(GL_TEXTURE5);
        roughness->bind();
    }
}
