#include <GL/glew.h>
#include <vector>
#include <string>

class Mesh {
    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        GLuint vao;
        GLuint vertexVbo;
        GLuint ebo;
        
        GLuint shaderProgram;
    
    public:
        Mesh(std::string filename);
        ~Mesh() {unloadVram();}

        void assignShader(GLuint program) {shaderProgram = program;}

        void render();
        void loadVram();
        void unloadVram();
};