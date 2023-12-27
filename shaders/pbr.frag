#version 330 core

// STRUCTS
struct DirectionLight {
	vec4 La;
	vec4 Ld;
	vec4 Ls;
	vec4 direction;
};

// LIGHTS UBO
layout(std140) uniform Lights{
	DirectionLight light;
};

// TEXTURES
uniform sampler2D albedoMap;
uniform sampler2D aoMap;
uniform sampler2D heightMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

// FRAGMENT SHADER IN
in vec2 uvCoords;

in WORLD_SPACE {
	vec4 vertexPosition;
	vec4 vertexNormal;
} worldSpace;

in CAMERA_SPACE {
	vec4 vertexPosition;
	vec4 vertexNormal;
	vec4 lightDirection;
} cameraSpace;

// FRAGMENT SHADER OUT
out vec4 fragmentColor;

const float PI = 3.14159265359;

float shadows() {
	return 1.0;
}

vec3 sRGB2Linear(vec3 color) {
	// return pow(color, 2.2);
	return color;
}

// PBR LIGHTING
// https://learnopengl.com/PBR/Lighting
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
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
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 pbrLighting(float visibility, vec3 defaultF0) {
	// Lighting parameters for fragment
	vec3 albedo     = sRGB2Linear(texture(albedoMap, uvCoords).rgb);
    // vec3 normal     = getNormalFromNormalMap();
    float metallic  = texture(metallicMap, uvCoords).r;
    float roughness = texture(roughnessMap, uvCoords).r;
    float ao        = texture(aoMap, uvCoords).r;

	vec3 N = normalize(cameraSpace.vertexNormal).xyz; // Fragment normal
	vec3 E = normalize(-cameraSpace.vertexPosition).xyz; // Eye vector

	vec3 F0 = mix(defaultF0, albedo, metallic);

	// Reflectance equation output
	vec3 Lo = vec3(0.0);

	vec3 L = normalize(cameraSpace.lightDirection).xyz; // Light vector
	vec3 H = normalize(L+E); // Halfway vector

	// Light parameters
	float distance = length(L);
	float attenuation = 1.0 / distance * distance;
	vec3 radiance = light.La.xyz * attenuation;

	// Cook-Torance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G   = GeometrySmith(N, E, L, roughness);
	vec3 F    = fresnelSchlick(max(dot(H, E), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(N, E), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular     = numerator / denominator;

	// add to outgoing radiance Lo
	float NdotL = max(dot(N, L), 0.0);
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;

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
