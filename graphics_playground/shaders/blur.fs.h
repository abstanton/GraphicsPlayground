const char* blur_fs = R"(
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform float radius;
uniform float kernel[25];

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    mat4 inverse_proj;
    vec4 cam_position;
};

layout(binding=0) uniform sampler2D input_texture;

void main()
{ 
    vec3 output_val = vec3(0);

    // Output pixel is linear combination of each pixel in the kernel
    float pixel_width = radius/5;
    for (int i = -2; i <= 2; i++) {
      for (int j = -2; j <= 2; j++) {
        float kernel_value = kernel[(i+2)+(5*(j+2))];
        vec2 offset = vec2(float(i)*pixel_width, float(j)*pixel_width);
        output_val += vec3(texture(input_texture, TexCoords+offset)*(kernel_value/273));
      }
    }

    FragColor = vec4(output_val, 1);
}

)";