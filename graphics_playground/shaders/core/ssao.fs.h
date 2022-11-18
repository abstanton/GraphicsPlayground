const char* ssao_fs = R"(
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
    float near_plane;
};

layout(binding=0) uniform sampler2D depth_texture;
layout(binding=1) uniform sampler2D normal_texture;

layout(binding = 5) uniform sampler2D ssao_noise;
uniform vec3 samples[64];

//https://www.khronos.org/opengl/wiki/Compute_eye_space_from_window_space
uniform vec2 viewport = vec2(1920, 1280);
const vec2 depthrange = vec2(0,1);

// TODO: Use more efficient version!
vec4 CalcEyeFromWindow(in vec3 windowSpace)
{
	vec3 ndcPos;
	ndcPos.xy = ((2.0 * windowSpace.xy) - (2.0 * 0)) / (viewport.xy) - 1;
	ndcPos.z = (2.0 * windowSpace.z - depthrange.x - depthrange.y) /
    (depthrange.y - depthrange.x);

	vec4 clipPos;
	clipPos.w = projection[3][2] / (ndcPos.z - (projection[2][2] / projection[2][3]));
	clipPos.xyz = ndcPos * clipPos.w;

	return inverse_proj * clipPos;
}

const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;
const vec2 noise_scale = vec2(1920, 1280) / 4.0;

void main()
{ 
    float depth = texture(depth_texture, gl_FragCoord.xy / viewport.xy).r;
    if (depth >= 0.9999f) {
        FragColor = vec4(vec3(1.0), 1.0);
        return;
    }

    vec3 fragPos   = CalcEyeFromWindow(vec3(gl_FragCoord.xy, depth)).xyz;

    vec3 normal = normalize(texture(normal_texture, TexCoords).rgb);
    vec3 randomVec = texture(ssao_noise, TexCoords * noise_scale).xyz * 2.0 - 1.0;  
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);  

    // ---------------- SSAO -----------------
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
        
        vec3 view_pos = CalcEyeFromWindow(vec3(offset.xy * viewport.xy, texture(depth_texture, offset.xy).r)).xyz;
        float sampleDepth = view_pos.z; 
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; 
    }  
    occlusion = 1.0 - (occlusion / kernelSize);
    // ------------------------------------

    FragColor = vec4(vec3(occlusion), 1.0);
}
)";