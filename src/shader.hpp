#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>

class Shader {
private:
    GLuint programId;

    GLuint matricesUBOIndex, lightsUBOIndex;
    GLuint albedoLocation, aoLocation, heightLocation, metallicLocation, normalLocation, roughnessLocation;
    GLuint MLocation;

    void createShaderProgram(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
    void getUniformLocations();

public:
    Shader(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
    ~Shader();

    //Default copy constructor/assignment.
    Shader(const Shader&) = default;
    Shader &operator=(const Shader&) = default;

    Shader(Shader&& other) :
        programId(other.programId),
        matricesUBOIndex(other.matricesUBOIndex),
        lightsUBOIndex(other.lightsUBOIndex),
        albedoLocation(other.albedoLocation),
        aoLocation(other.aoLocation),
        heightLocation(other.heightLocation),
        metallicLocation(other.metallicLocation),
        normalLocation(other.normalLocation),
        roughnessLocation(other.roughnessLocation),
        MLocation(other.MLocation)
    {
        other.programId = 0; //Use the "null" programId for the old object.
    }

    Shader &operator=(Shader&& other) {
        //ALWAYS check for self-assignment.
        if(this != &other)
        {
            releaseShader();
            std::swap(programId, other.programId);
            std::swap(matricesUBOIndex, other.matricesUBOIndex);
            std::swap(lightsUBOIndex, other.lightsUBOIndex);
            std::swap(albedoLocation, other.albedoLocation);
            std::swap(aoLocation, other.aoLocation);
            std::swap(heightLocation, other.heightLocation);
            std::swap(metallicLocation, other.metallicLocation);
            std::swap(normalLocation, other.normalLocation);
            std::swap(roughnessLocation, other.roughnessLocation);
            std::swap(MLocation, other.MLocation);
        }

        return *this;
    }

    void releaseShader();

    GLuint getMLocation() {return MLocation;}

    GLuint getProgram() {return programId;}
};

#endif
