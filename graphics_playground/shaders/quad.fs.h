#pragma once

const char* quad_fs = R"(
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform float gamma = 2.2;
uniform float exposure = 1.0;

layout(binding=0) uniform sampler2D screen_texture;
layout(binding=1) uniform sampler2D depth_texture;

void main()
{ 
    vec3 color = texture(screen_texture, TexCoords).rgb;
    vec4 depth = texture(depth_texture, TexCoords);
    color = vec3(1.0) - exp(-color * exposure);
    color = pow(color, vec3(1.0/gamma)); 
    FragColor = vec4(color, 1.0);
}
)";