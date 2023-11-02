#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>

class Shader {
    private:
        GLuint programId;

        GLuint vertId;
        GLuint fragId;
        GLuint geomId;

    public:
        Shader(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
        ~Shader();

        GLuint getProgram() {return programId;}
};

#endif