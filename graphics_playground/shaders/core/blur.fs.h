const char* blur_fs = R"(
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform float radius;

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    mat4 inverse_proj;
    vec4 cam_position;
};

layout(binding=0) uniform sampler2D input_texture;

// https://github.com/Jam3/glsl-fast-gaussian-blur
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

void main()
{ 
    vec4 output_val;
    output_val = blur13(input_texture, TexCoords, textureSize(input_texture, 0), vec2(0, radius));
    output_val += blur13(input_texture, TexCoords, textureSize(input_texture, 0), vec2(radius, 0));
    output_val /= 2;

    FragColor = vec4(output_val.xyz, 1);
}

)";