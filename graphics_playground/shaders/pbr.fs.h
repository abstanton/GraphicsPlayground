#pragma once

const char* pbr_fs = R"(#version 460
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;
            
in vec2 uv;
in vec3 norm;
in vec3 pos;
in mat3 TBN;

#define MAX_TOTAL_POINT_LIGHTS 100
#define MAX_TOTAL_DIRECTION_LIGHTS 100
#define MAX_TOTAL_DIRECTION_SHADOWS 10

struct PointLight {
    vec4 position;
    vec4 colour;
    float intensity;
};

struct DirectionLight {
    vec4 direction;
    vec4 colour;
    float intensity;
    int is_shadowed;
    mat4 light_space_matrix;
};

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    vec4 cam_position;
};

layout(std140, binding = 1) uniform Lights {
    vec4 ambientLight;
    int numPointLights;
    int numDirectionLights;
    PointLight point_lights[MAX_TOTAL_POINT_LIGHTS];
    DirectionLight direction_lights[MAX_TOTAL_DIRECTION_LIGHTS];
};

uniform int diffuse_use_tex;
layout(bindless_sampler) uniform sampler2D diffuse_tex;
uniform vec3 diffuse_val;
uniform vec2 diffuse_scale;

uniform int roughness_use_tex;
layout(bindless_sampler) uniform sampler2D roughness_tex;
uniform float roughness_val;
uniform vec2 roughness_scale;

uniform int normal_use_tex;
layout(bindless_sampler) uniform sampler2D normal_tex;
uniform vec3 normal_val;
uniform vec2 normal_scale;

uniform int metalness_use_tex;
layout(bindless_sampler) uniform sampler2D metalness_tex;
uniform float metalness_val;
uniform vec2 metalness_scale;

layout(binding = 10) uniform sampler2DArray shadowMap;

const float PI = 3.14159265359;

float ShadowCalculation(vec4 fragPosLightSpace, float i)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
  
    float currentDepth = projCoords.z;

	float bias = 0.005;
   
	float shadow = 0.0;
	
    vec2 texelSize = vec2(1)/textureSize(shadowMap, 0).xy;

    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + (vec2(x, y) * texelSize), i)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow = clamp(shadow / 25.0, 0.0, 1.0);
	if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  


void main()
{
    vec3 albedo = diffuse_use_tex == 1? vec3(texture(diffuse_tex, uv*diffuse_scale)) : diffuse_val;
    float roughness = roughness_use_tex == 1? vec3(texture(roughness_tex, uv*roughness_scale)).x : roughness_val;
	
    vec3 normal;
    if (normal_use_tex == 1) {
        normal = texture(normal_tex, uv*normal_scale).rgb;
        normal = normal* 2.0 - 1.0;
        normal = normalize(TBN * normal);
    }
    else {
        normal = norm;
    }
    
    float metallic = metalness_use_tex == 1? vec3(texture(metalness_tex, uv*metalness_scale)).x : metalness_val;

	vec3 V = normalize(vec3(cam_position) - pos);    
    vec3 N = normal;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < numPointLights; i++) {
        PointLight light = point_lights[i];
        vec3 position = vec3(light.position.x, light.position.y, light.position.z);
        vec3 colour = vec3(light.colour.x, light.colour.y, light.colour.z);

        float distance = length(position - pos);
        float attenuation = 1/(distance*distance);
        vec3 radiance = colour * attenuation * light.intensity;

        vec3 L = normalize(position - pos);
        vec3 H = normalize(V + L);

		float NDF = DistributionGGX(N, H, roughness);
        float G  = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

		vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

	for (int i = 0; i < numDirectionLights; i++) {
        DirectionLight light = direction_lights[i];
		vec4 fragPosLightSpace = light.light_space_matrix * vec4(pos, 1.0);
        float shadow = ShadowCalculation(fragPosLightSpace, i);

        vec3 colour = vec3(light.colour.x, light.colour.y, light.colour.z);
    
        vec3 radiance = colour * (1-shadow);

        vec3 L = normalize(vec3(light.direction));
        vec3 H = normalize(V + L);

		float NDF = DistributionGGX(N, H, roughness);
        float G  = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

		vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

	vec3 ambient = vec3(0.03) * albedo * vec3(ambientLight);
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);
})";