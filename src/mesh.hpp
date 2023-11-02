#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <shader.hpp>

class Mesh {
    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> faces;
        std::vector<GLfloat> colors;

        GLuint VAO;
        GLuint vertexVBO;
        GLuint colorVBO;
        GLuint EBO;
        
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
    
    public:
        Mesh();
        Mesh(std::string filename);
        ~Mesh() {unloadVram();}

        void loadVram();
        void unloadVram();

        glm::mat4 modelMatrix();

        int vertexCount() {return vertices.size();}
        GLuint getVao() {return VAO;}

        void draw();
};

#endif