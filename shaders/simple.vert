#version 330 core

// VERTEX SHADER IN
layout(location = 0) in vec3 vertexPosition;

// MATRICES UBO
layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

// MODEL MATRIX
uniform mat4 M;

void main() {
    gl_Position = P * V * M * vec4(vertexPosition, 1.0);
}
