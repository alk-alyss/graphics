#version 330 core

// STRUCTS
struct DirectionLight {
    vec4 colorPower; // first three floats are color, last one is power
    vec4 direction;
};

// VERTEX SHADER IN
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

// MATRICES UBO
layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

#define MAX_DIR_LIGHTS 2

// LIGHTS UBO
layout(std140) uniform Lights {
    DirectionLight lights[MAX_DIR_LIGHTS];
};

// MODEL MATRIX
uniform mat4 M;

// TEXTURES
uniform sampler2D albedo;
uniform sampler2D ao;
uniform sampler2D height;
uniform sampler2D metallic;
uniform sampler2D normal;
uniform sampler2D roughness;

// VERTEX SHADER OUT
out vec2 uvCoords;

out WORLD_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
} worldSpace;

out CAMERA_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
    vec4 lightDirections[MAX_DIR_LIGHTS];
} cameraSpace;


void main() {
    worldSpace.vertexPosition = M * vec4(vertexPosition, 1);
    worldSpace.vertexNormal = M * vec4(vertexNormal, 0);

    cameraSpace.vertexPosition = V * worldSpace.vertexPosition;
    cameraSpace.vertexNormal = V * worldSpace.vertexNormal;

    for (int i=0; i<MAX_DIR_LIGHTS; i++) {
        cameraSpace.lightDirections[i] = V * lights[i].direction;
    }

    uvCoords = vertexUV;

    gl_Position = P * cameraSpace.vertexPosition;
}
