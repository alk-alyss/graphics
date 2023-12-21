#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>

class Shader {
private:
    GLuint programId;

    GLuint vertId, fragId, geomId;

    GLuint matricesUBOIndex, lightsUBOIndex, materialUBOIndex;
    GLuint MLocation;

public:
    Shader(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
    ~Shader();

    GLuint getMLocation() {return MLocation;}

    GLuint getProgram() {return programId;}
};

#endif
