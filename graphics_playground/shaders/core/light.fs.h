#pragma once
const char* light_fs = R"(#version 460
out vec4 FragColor;

uniform vec3 colour_val;

void main()
{
    FragColor = vec4(colour_val, 1.0);
})";