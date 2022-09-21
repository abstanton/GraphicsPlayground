#pragma once
const char* shadow_fs = R"(#version 460

void main()
{             
  gl_FragDepth = gl_FragCoord.z;
})";
