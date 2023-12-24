#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include <SOIL2.h>

class Texture {
private:
    GLuint textureId = 0;
    std::string path;

public:
    Texture(const std::string imagePath);
    ~Texture();

    std::string getPath() {return path;}

    void bind();
};

#endif
