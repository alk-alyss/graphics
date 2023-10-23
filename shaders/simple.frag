#version 330 core

// task define input variable from vertex shader
in vec3 color;

// task define output variable of the fragment color
out vec3 fragmentColor;

void main()
{
    // task assign color to fragment output variable 
	fragmentColor = color;
}