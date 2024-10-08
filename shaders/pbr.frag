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

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 1

// LIGHTS UBO
layout(std140) uniform Lights{
	DirectionLight dirLights[MAX_DIR_LIGHTS];
	PointLight pointLights[MAX_POINT_LIGHTS];
	int dirLightsCount;
	int pointLightsCount;
};

// Camera Position
uniform vec3 cameraPosition;

// TEXTURES
uniform sampler2D albedoMap;
uniform sampler2D aoMap;
uniform sampler2D heightMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

// FRAGMENT SHADER IN
in vec2 uvCoords;
in mat3 TBN;

in WORLD_SPACE {
	vec3 position;
	vec3 normal;
	vec3 tangent;
} worldSpace;

in CAMERA_SPACE {
	vec3 position;
	vec3 normal;
	vec3 lightDirections[MAX_DIR_LIGHTS];
	vec3 lightPositions[MAX_POINT_LIGHTS];
} cameraSpace;

// FRAGMENT SHADER OUT
out vec4 fragmentColor;

const float PI = 3.14159265359;

float shadows() {
	return 1.0;
}

// Normal mapping
//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
vec3 tangentSpaceNormal(vec3 normal) {
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);
	return normal;
}

// PBR LIGHTING
// https://learnopengl.com/PBR/Lighting
float DistributionGGX(float NdotH, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float ggx1  = GeometrySchlickGGX(NdotV, roughness);
    float ggx2  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CookToranceBRDF(
		vec3 N,
		vec3 V,
		vec3 L,
		vec3 H,
		vec3 radiance,
		vec3 albedo,
		float metallic,
		float roughness,
		vec3 F0
	) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);
	float VdotH = max(dot(V, H), 0.0);

	float NDF = DistributionGGX(NdotV, roughness);
	float G   = GeometrySmith(NdotV, NdotL, roughness);
	vec3 F    = fresnelSchlick(VdotH, F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular     = numerator / denominator;

	specular *= metallic;

	vec3 diffuse = kD * albedo / PI;

	// add to outgoing radiance Lo
	return (diffuse + specular) * radiance * NdotL;
}

vec3 pbrLighting(float visibility, vec3 defaultF0) {
	// Lighting parameters for fragment
	vec3 albedo     = texture(albedoMap, uvCoords).rgb;
    float metallic  = texture(metallicMap, uvCoords).r;
	vec3 normal = texture(normalMap, uvCoords).rgb;
    float roughness = texture(roughnessMap, uvCoords).r;
    float ao        = texture(aoMap, uvCoords).r;

	// vec3 N = normalize(worldSpace.normal); // Fragment normal
	vec3 N = tangentSpaceNormal(normal);
	vec3 V = normalize(cameraPosition - worldSpace.position); // View vector

	vec3 F0 = mix(defaultF0, albedo, metallic);

	// Reflectance equation output
	vec3 Lo = vec3(0.0);

	for (int i=0; i<dirLightsCount; i++) {
		vec3 L = normalize(-dirLights[i].direction.xyz); // Light vector
		vec3 H = normalize(V+L); // Halfway vector

		vec3 radiance = dirLights[i].colorPower.xyz * dirLights[i].colorPower.w;

		Lo += CookToranceBRDF(N, V, L, H, radiance, albedo, metallic, roughness, F0);
	}

	for (int i=0; i<pointLightsCount; i++) {
		vec3 L = pointLights[i].position.xyz - worldSpace.position; // Light vector
		float distance = length(L);

		L = normalize(L); // Normalized light vector
		vec3 H = normalize(V+L); // Halfway vector

		float attenuation = 1.0 / (distance * distance); // inverse square law
		vec3 radiance = pointLights[i].colorPower.xyz * pointLights[i].colorPower.w * attenuation;

		Lo += CookToranceBRDF(N, V, L, H, radiance, albedo, metallic, roughness, F0);
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

	return color;
}

vec3 hdrToneMapping(vec3 color) {
	return color / (color + vec3(1.0));
}

vec3 gammaCorrection(vec3 color) {
	return pow(color, vec3(1.0/2.2));
}

void main() {
	float visibility = shadows();

	vec3 color = pbrLighting(visibility, vec3(0.4));
	color = hdrToneMapping(color);
	color = gammaCorrection(color);

	fragmentColor = vec4(color, 1.0);
}
