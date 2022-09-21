#pragma once

const char* quad_fs = R"(
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform float gamma = 2.2;
uniform float exposure = 1.0;

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    mat4 inverse_proj;
    vec4 cam_position;
};

layout(binding=0) uniform sampler2D screen_texture;
layout(binding=1) uniform sampler2D depth_texture;
layout(binding=2) uniform sampler2D normal_texture;
layout(binding=3) uniform sampler2D pos_texture;

layout(binding = 5) uniform sampler2D ssao_noise;
uniform vec3 samples[64];

vec3 viewPosFromDepth(float depth) {
    float x = TexCoords.x;
    float y = TexCoords.y;
    vec4 proj_pos = vec4(x,y,depth,1.0);
    vec4 view_pos = inverse_proj * proj_pos;
    return view_pos.xyz / view_pos.w;
}

const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;
const vec2 noise_scale = vec2(1920, 1280) / 4.0;

void main()
{ 
    float depth = texture(depth_texture, TexCoords).r * 2.0 - 1.0;
    vec3 fragPos = texture(pos_texture, TexCoords).xyz;

    //Ideally we want to calculate view space here
    //vec3 fragPos   = viewPosFromDepth(depth);

    vec3 normal = normalize(texture(normal_texture, TexCoords).rgb);
    vec3 randomVec = texture(ssao_noise, TexCoords * noise_scale).xyz * 2.0 - 1.0;  
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);  

    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space
        samplePos = fragPos + samplePos * radius; 
        
        vec4 offset = vec4(samplePos, 1.0);
        offset      = projection * offset;    // from view to clip-space
        offset.xyz /= offset.w;               // perspective divide
        offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  
        float sampleDepth = texture(pos_texture, offset.xy).z; 
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; 
    }  
    occlusion = 1.0 - (occlusion / kernelSize);
    vec3 color = texture(screen_texture, TexCoords).rgb;
    color = vec3(1.0) - exp(-color * exposure);

    color = pow(color, vec3(1.0/gamma)); 
    FragColor = vec4(color * occlusion, 1.0);
    //FragColor = vec4(vec3(occlusion), 1.0);
    //FragColor = vec4(texture(pos_texture, TexCoords).xyz, 1.0);
    //FragColor = vec4(texture(normal_texture, TexCoords).xyz, 1.0);
    //FragColor = vec4(fragPos,1.0);
    //FragColor = vec4(vec3(depth), 1.0);
}
)";