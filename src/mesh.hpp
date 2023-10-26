#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

class Mesh {
    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> faces;
        std::vector<GLfloat> colors;

        GLuint vao;
        GLuint vertexVbo;
        GLuint colorVbo;
        GLuint ebo;
        
        GLuint shaderProgram;
        glm::mat4 modelMatrix;
        GLuint modelMatrixLocation;
    
    public:
        Mesh();
        Mesh(std::string filename);
        // ~Mesh() {unloadVram();}

        void assignShader(GLuint program);

        void render();
        void loadVram();
        void unloadVram();
};

#endif