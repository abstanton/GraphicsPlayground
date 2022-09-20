#pragma once

const char* quad_fs = R"(
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

layout(binding=0) uniform sampler2D screenTexture;

void main()
{ 
    vec4 frag_out = texture(screenTexture, TexCoords);
    frag_out.w = 1.0;
    FragColor = frag_out;
}
)";