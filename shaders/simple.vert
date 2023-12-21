#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

layout(std140) uniform Matrices {
    mat4 V;
    mat4 P;
};

uniform mat4 M;

out VERTEX_WORLD {
    vec4 position;
    vec4 normal;
    vec2 uv;
} vertexWorld;

void main()
{
    vertexWorld.position = M * vec4(vertexPosition, 1);
    vertexWorld.normal = M * vec4(vertexNormal, 0);
    vertexWorld.uv = vertexUV;

    gl_Position = P * V * vertexWorld.position;
}
