#pragma once

/*
https://www.youtube.com/watch?v=NCptEJ1Uevg&ab_channel=OGLDEV pcf with random
sampling
*/

const char* pbr_fs = R"(#version 460
#extension GL_ARB_bindless_texture : require

layout(location = 0) out vec4 lighting_color;
layout(location = 1) out vec3 normal_color;
            
in vec2 uv;
in vec3 pos;
in mat3 TBN;
in vec3 view_pos;
in vec3 view_norm;
in mat3 view_model_mat;

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
    float size;
    float frustrum_width;
};

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    mat4 inverse_proj;
    vec4 cam_position;
    float near_plane;
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

layout(binding = 10) uniform sampler2DArray shadow_map_array;
layout(binding = 11) uniform sampler2D depth_map;
layout(binding = 12) uniform sampler2D disk_samples;
layout(binding = 13) uniform sampler2D noise;

uniform int pcf_samples = 64;
uniform vec2 viewport;
uniform float pcf_scale_fac = 0.0013f;

const float PI = 3.14159265359;

mat2 RotationMat(float angle) {
    return mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 light_dir, float width, float size, float i)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
  
    float currentDepth = projCoords.z;

	float bias = max(0.03 * (1.0 - dot(normal, light_dir)), 0.0001);  

    vec2 texelSize = vec2(1)/textureSize(shadow_map_array, 0).xy;
    float light_size_uv =  size / width;
    float searchWidth = light_size_uv * (currentDepth - 1.0f) / currentDepth;
    float blocker_depth = 0;
    int blocker_num = 0;

    for (float u = -2; u < 3; u++) {
        for (float v = -2; v < 3; v++) {
            vec2 offset = vec2(u, v) * searchWidth;
            float depth = texture(shadow_map_array, vec3(projCoords.xy + offset, i)).r; 
            if (depth < currentDepth) {
                blocker_depth += depth;
                blocker_num += 1;
            }    
        } 
    }

    blocker_depth = blocker_depth / float(blocker_num);
    float penumbra = (currentDepth - blocker_depth) / blocker_depth;
    penumbra = penumbra * light_size_uv * 1.0 / currentDepth;

    float noise_size = textureSize(noise, 0).x;
    float noise_uv_scale = viewport.x * viewport.y / noise_size;
    float x_pix = gl_FragCoord.x;
    float y_pix = gl_FragCoord.y;
    float noise_uv = (x_pix*y_pix)/(viewport.x*viewport.y) * noise_uv_scale;
    float noise_val = texture(noise, vec2(noise_uv, 0)).x;
    mat2 rot_mat = RotationMat(2 * PI * noise_val);

    float disk_tex_size = 1.0f/1024.0f;
	float shadow = 0.0;

    for (int j = 0; j < pcf_samples*2; j+=2) {
        float offset_x = texture(disk_samples, vec2(float(j)*disk_tex_size, 0.5)).x;
        float offset_y = texture(disk_samples, vec2(float((j+1))*disk_tex_size, 0.5)).x;
        vec2 offset = vec2(offset_x, offset_y);
        offset = rot_mat * offset;
        float pcf_depth = texture(shadow_map_array, vec3(projCoords.xy + (offset*pcf_scale_fac), i)).r; 
        shadow += currentDepth - bias > pcf_depth ? 1.0 : 0.0;     
    }

    shadow = clamp(shadow / float(pcf_samples), 0.0, 1.0);

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

// Return light contribution
// param: radiance: radiance of light incidident at position
// param: L: view space direciton from frag to light
// param: V: view space vector from camera to frag
// param: N: view space normal at frag
// param: F0: Reflection at zero incidence for material
// param: albedo: albedo colour at surface
// param: roughness: roughness of surface
// param: metalness: metalness of surface
vec3 lightingCalc(vec3 radiance, vec3 L, vec3 V, vec3 N, vec3 F0, vec3 albedo, float roughness, float metallic) {
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
        
    float NdotL = max(dot(N, L), 0.0);          

    return (kD * albedo / PI + specular) * radiance * NdotL; 
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
        normal = view_norm;
    }

    vec3 world_norm = inverse(view_model_mat) * view_norm;
    
    float metallic = metalness_use_tex == 1? vec3(texture(metalness_tex, uv*metalness_scale)).x : metalness_val;

	vec3 V = (view * vec4(normalize(vec3(cam_position) - pos), 0.0)).xyz;    
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

        vec3 L = normalize((view*vec4(vec3(position - pos), 0.0)).xyz);;
        Lo += lightingCalc(radiance, L, V, N, F0, albedo, roughness, metallic);
    }

	for (int i = 0; i < numDirectionLights; i++) {
        DirectionLight light = direction_lights[i];
		vec4 fragPosLightSpace = light.light_space_matrix * vec4(pos, 1.0);
        float shadow = ShadowCalculation(fragPosLightSpace, world_norm, normalize(-light.direction.xyz),  light.frustrum_width, light.size, i);
        vec3 colour = vec3(light.colour.x, light.colour.y, light.colour.z);
    
        vec3 radiance = colour * (1-shadow) * light.intensity;

        vec3 L = normalize((view*vec4(vec3(-light.direction), 0.0)).xyz);
        Lo += lightingCalc(radiance, L, V, N, F0, albedo, roughness, metallic);
    }

	vec3 ambient = vec3(0.03) * albedo * vec3(ambientLight);
    vec3 color = ambient + Lo; 
   
    lighting_color = vec4(color, 1.0);
    normal_color = normal;
})";