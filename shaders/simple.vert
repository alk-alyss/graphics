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

// UBOs
layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

layout(std140) uniform Lights {
    DirectionLight light;
};

// UNIFORMS
uniform mat4 M;

// VERTEX SHADER OUT
out WORLD_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
    vec2 uv;
} worldSpace;

out CAMERA_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
    vec4 lightDirection;
} cameraSpace;


void main() {
    worldSpace.vertexPosition = M * vec4(vertexPosition, 1);
    worldSpace.vertexNormal = M * vec4(vertexNormal, 0);
    worldSpace.uv = vertexUV;

    cameraSpace.vertexPosition = V * worldSpace.vertexPosition;
    cameraSpace.vertexNormal = V * worldSpace.vertexNormal;
    cameraSpace.lightDirection = V * light.direction;

    gl_Position = P * cameraSpace.vertexPosition;
}
