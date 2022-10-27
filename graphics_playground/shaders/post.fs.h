#pragma once

const char* post_fs = R"(
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
layout(binding=1) uniform sampler2D ao_texture;

void main()
{ 
    vec3 color = texture(screen_texture, TexCoords).rgb;
    float ao = texture(ao_texture, TexCoords).x;

    // Gamma correction and tone mapping
    color = vec3(1.0) - exp(-color * exposure);
    color = pow(color, vec3(1.0/gamma)); 
    FragColor = vec4(color * ao, 1.0);
}
)";