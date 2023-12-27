#version 330 core

// STRUCTS
struct DirectionLight {
    vec4 La;
    vec4 Ld;
    vec4 Ls;
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

// LIGHTS UBO
layout(std140) uniform Lights {
    DirectionLight light;
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
    vec4 lightDirection;
} cameraSpace;


void main() {
    worldSpace.vertexPosition = M * vec4(vertexPosition, 1);
    worldSpace.vertexNormal = M * vec4(vertexNormal, 0);

    cameraSpace.vertexPosition = V * worldSpace.vertexPosition;
    cameraSpace.vertexNormal = V * worldSpace.vertexNormal;
    cameraSpace.lightDirection = V * light.direction;

    uvCoords = vertexUV;

    gl_Position = P * cameraSpace.vertexPosition;
}
