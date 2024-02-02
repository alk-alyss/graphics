#version 330 core

// STRUCTS
struct DirectionLight {
    vec4 colorPower; // first three floats are color, last one is power
    vec4 direction;
};

struct PointLight {
    vec4 colorPower; // first three floats are color, last one is power
    vec4 position;
};

// VERTEX SHADER IN
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

// MATRICES UBO
layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 1

// LIGHTS UBO
layout(std140) uniform Lights {
    DirectionLight dirLights[MAX_DIR_LIGHTS];
    PointLight pointLights[MAX_POINT_LIGHTS];
    int dirLightsCount;
    int pointLightsCount;
};

// Camera Position
uniform vec3 cameraPosition;

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
    vec3 vertexPosition;
    vec3 vertexNormal;
} worldSpace;

out CAMERA_SPACE {
    vec3 vertexPosition;
    vec3 vertexNormal;
    vec3 lightDirections[MAX_DIR_LIGHTS];
    vec3 lightPositions[MAX_POINT_LIGHTS];
} cameraSpace;


void main() {
    vec4 vertexPosition_worldSpace = M * vec4(vertexPosition, 1);
    vec4 vertexNormal_worldSpace = normalize(M * vec4(vertexNormal, 0));

    vec4 vertexPosition_cameraSpace = V * vertexPosition_worldSpace;
    vec4 vertexNormal_cameraSpace = normalize(V * vertexNormal_worldSpace);

    cameraSpace.vertexPosition = vertexPosition_cameraSpace.xyz;
    cameraSpace.vertexNormal = (V * vertexNormal_worldSpace).xyz;

    for (int i=0; i<dirLightsCount; i++) {
        cameraSpace.lightDirections[i] = (V * dirLights[i].direction).xyz;
    }

    for (int i=0; i<pointLightsCount; i++) {
        cameraSpace.lightPositions[i] = (V * pointLights[i].position).xyz;
    }

    worldSpace.vertexPosition = vertexPosition_worldSpace.xyz;
    worldSpace.vertexNormal = vertexNormal_worldSpace.xyz;

    cameraSpace.vertexPosition = vertexPosition_cameraSpace.xyz;
    cameraSpace.vertexNormal = vertexNormal_cameraSpace.xyz;

    uvCoords = vertexUV;

    gl_Position = P * vertexPosition_cameraSpace;
}
