#version 330 core

in VERTEX_WORLD {
	vec4 position;
	vec4 normal;
	vec2 uv;
} fragmentWorld;

out vec4 fragmentColor;

void main()
{
	fragmentColor = fragmentWorld.normal;
}
