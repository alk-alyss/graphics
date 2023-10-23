#version 330 core

// task define attribute input
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

// task define output variable (for the color)
out vec3 color;

void main()
{
    // task assign homogeneous position to gl_Position
	//gl_Position.xyz = vertexPosition_modelspace;
	//gl_Position.w = 1.0;

	gl_Position = vec4(vertexPosition_modelspace, 1.0);

    // task propagate color to fragment shader
	color = vertexColor;

    // task change the size of points (gl_PointSize)
    gl_PointSize = 10;
}
