#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define UV_LOCATION 2
#define TANGENT_LOCATION 3
#define INSTANCE_MATRIX_LOCATION 4

class Shader {
private:
    GLuint programId;

    GLuint matricesUBOIndex, lightsUBOIndex;
    GLuint albedoLocation, aoLocation, heightLocation, metallicLocation, normalLocation, roughnessLocation;
    GLuint MLocation;
    GLuint cameraPositionLocation;

    void createShaderProgram(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
    void getUniformLocations();

public:
    Shader() = delete;
    Shader(const std::string vertFile, const std::string fragFile, const std::string geomFile="");
    ~Shader();

    //Delete copy constructor/assignment.
    Shader(const Shader& other) = delete;
    Shader &operator=(const Shader& other) = delete;

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
        MLocation(other.MLocation),
        cameraPositionLocation(other.cameraPositionLocation)
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
            std::swap(cameraPositionLocation, other.cameraPositionLocation);
        }

        return *this;
    }

    void bind() const;

    void releaseShader();

    GLuint getMLocation() const {return MLocation;}
    GLuint getCameraPositionLocation() const {return cameraPositionLocation;}

    GLuint getProgram() const {return programId;}
};

#endif
