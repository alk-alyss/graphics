#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "shader.hpp"

void compileShader(GLuint& shaderID, const std::string file) {
    // read shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        throw std::runtime_error(std::string("Can't open shader file: ") + file);
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // compile Vertex Shader
    std::cout << "Compiling shader: " << file << std::endl;
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // check Vertex Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
        //throw runtime_error(string(&shaderErrorMessage[0]));
        std::cout << &shaderErrorMessage[0] << std::endl;
    }
}

void Shader::createShaderProgram(
    const std::string vertexFilePath,
    const std::string fragmentFilePath,
    const std::string geometryFilePath
) {
    // Create the shaders
    GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertId, vertexFilePath);

    GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragId, fragmentFilePath);

    GLuint geomId = 0;
    if (geometryFilePath != "") {
        geomId = glCreateShader(GL_GEOMETRY_SHADER);
        compileShader(geomId, geometryFilePath);
    }

    // Link the program
    std::cout << "Linking shaders... " << std::endl;
    programId = glCreateProgram();
    glAttachShader(programId, vertId);
    if (geometryFilePath != "")
        glAttachShader(programId, geomId);
    glAttachShader(programId, fragId);
    glLinkProgram(programId);

    // Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
        throw std::runtime_error(std::string(&programErrorMessage[0]));
    }

    glDetachShader(programId, vertId);
    glDeleteShader(vertId);

    if (geometryFilePath != "") {
        glDetachShader(programId, geomId);
        glDeleteShader(geomId);
    }

    glDetachShader(programId, fragId);
    glDeleteShader(fragId);

    std::cout << "Shader program complete." << std::endl;
}

void Shader::getUniformLocations() {
    matricesUBOIndex = glGetUniformBlockIndex(programId, "Matrices");
    glUniformBlockBinding(programId, matricesUBOIndex, 0);

    lightsUBOIndex = glGetUniformBlockIndex(programId, "Lights");
    glUniformBlockBinding(programId, lightsUBOIndex, 1);

    MLocation = glGetUniformLocation(programId, "M");

    cameraPositionLocation = glGetUniformLocation(programId, "cameraPosition");

    albedoLocation = glGetUniformLocation(programId, "albedoMap");
    aoLocation = glGetUniformLocation(programId, "aoMap");
    heightLocation = glGetUniformLocation(programId, "heightMap");
    metallicLocation = glGetUniformLocation(programId, "metallicMap");
    normalLocation = glGetUniformLocation(programId, "normalMap");
    roughnessLocation = glGetUniformLocation(programId, "roughnessMap");
}

Shader::Shader(
    const std::string vertexFilePath,
    const std::string fragmentFilePath,
    const std::string geometryFilePath
) {
    createShaderProgram(vertexFilePath, fragmentFilePath, geometryFilePath);
    getUniformLocations();
}

Shader::~Shader() {
    releaseShader();
}

void Shader::bind() {
    glUseProgram(programId);

    glUniform1i(albedoLocation, 0);
    glUniform1i(aoLocation, 1);
    glUniform1i(heightLocation, 2);
    glUniform1i(metallicLocation, 3);
    glUniform1i(normalLocation, 4);
    glUniform1i(roughnessLocation, 5);
}

void Shader::releaseShader() {
    glDeleteProgram(programId);
}
