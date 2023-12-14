#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 mvp;

out vec3 color;

void main()
{
    gl_PointSize = 10;

    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0);

	color = vertexColor;
}
