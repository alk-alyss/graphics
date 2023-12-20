#include "texture.hpp"

#include <iostream>

using namespace std;

Texture::Texture(const string imagePath) {
    cout << "Reading image: " << imagePath << endl;

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

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}
