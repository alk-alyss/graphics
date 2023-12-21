#version 330 core

struct DirectionLight {
    vec4 La;
    vec4 Ld;
    vec4 Ls;
    vec4 position;
};

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

// layout(std140) uniform Lights {
//     DirectionLight light;
// };


uniform DirectionLight light;
uniform mat4 M;


out WORLD_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
    vec2 uv;
} worldSpace;

out CAMERA_SPACE {
    vec4 vertexPosition;
    vec4 vertexNormal;
    vec4 lightPosition;
} cameraSpace;


void main() {
    worldSpace.vertexPosition = M * vec4(vertexPosition, 1);
    worldSpace.vertexNormal = M * vec4(vertexNormal, 0);
    worldSpace.uv = vertexUV;

    cameraSpace.vertexPosition = V * worldSpace.vertexPosition;
    cameraSpace.vertexNormal = V * worldSpace.vertexNormal;
    cameraSpace.lightPosition = V * light.position;

    gl_Position = P * cameraSpace.vertexPosition;
}
