#version 330 core

// STRUCTS
struct DirectionLight {
	vec4 La;
	vec4 Ld;
	vec4 Ls;
	vec4 direction;
};

struct Material {
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float Ns;
};

// UBOs
layout(std140) uniform Lights{
	DirectionLight light;
};

layout(std140) uniform Materials {
	Material material;
};

// FRAGMENT SHADER IN
in WORLD_SPACE {
	vec4 vertexPosition;
	vec4 vertexNormal;
	vec2 uv;
} worldSpace;

in CAMERA_SPACE {
	vec4 vertexPosition;
	vec4 vertexNormal;
	vec4 lightDirection;
} cameraSpace;

// FRAGMENT SHADER OUT
out vec4 fragmentColor;

float shadows() {
	return 1.0;
}

// BLINN-PHONG LIGHTING
void lighting(float visibility) {

	vec4 _Ks = material.Ks;
	vec4 _Kd = material.Kd;
	vec4 _Ka = material.Ka;
	float _Ns = material.Ns;

	// use texture for materials
	// if (useTexture == 1) {
	// 	_Ks = vec4(texture(specularColorSampler, vertex_UV).rgb, 1.0);
	// 	_Kd = vec4(texture(diffuseColorSampler, vertex_UV).rgb, 1.0);
	// 	_Ka = vec4(0.05 * _Kd.rgb, _Kd.a);
	// 	_Ns = 10;
	// }

	vec4 Ia;
	vec4 Is;
	vec4 Id;

	// Ambient intensity (Ia)
	Ia += light.La * _Ka;

	// Diffuse intensity (Id)
	vec4 N = normalize(cameraSpace.vertexNormal); // Fragment normal
	vec4 L = normalize(cameraSpace.lightDirection); // Light vector
	float cosTheta = clamp(dot(N, L), 0, 1);
	Id += light.Ld * _Kd * cosTheta;

	// Specular intensity (Is)
	vec4 E = normalize(-cameraSpace.vertexPosition); // Eye vector
	vec4 H = normalize(L+E); // Halfway vector
	float cosAlpha = clamp(dot(N, H), 0, 1);
	float specular_factor = pow(cosAlpha, _Ns);
	Is += light.Ls * _Ks * specular_factor;

	// here we would normally model the light attenuation effect
	// but since this is a directional light source that is infinitely far away
	// attenuating the light intensity does not make sense

	// final fragment color
	fragmentColor = vec4(
		Ia +
		Id * visibility +
		Is * visibility
	);
}

void main() {
	float visibility = shadows();
	lighting(visibility);
}
