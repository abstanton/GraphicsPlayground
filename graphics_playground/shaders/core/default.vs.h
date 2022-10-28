const char* def_vs = R"(#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTang;
layout (location = 3) in vec2 aUV;

out vec2 uv;
out vec3 pos;
out mat3 TBN;
out vec3 view_pos;
out vec3 view_norm;

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    mat4 inverse_proj;
    vec4 cam_position;
};

uniform mat4 model;

void main()
{
   mat3 viewModelVector = transpose(inverse(mat3(view*model)));

   vec3 bitang = normalize(cross(normalize(aNorm), normalize(aTang)));

   vec3 T = normalize(viewModelVector * aTang);
   vec3 B = normalize(viewModelVector * bitang);
   vec3 N = normalize(viewModelVector * aNorm);

   TBN = mat3(T,B,N);

   uv = aUV;  
   view_norm = normalize(viewModelVector * aNorm);

   pos = vec3(model * vec4(aPos, 1.0));

   //Currently need to output view space position for SSAO
   view_pos = (view * model * vec4(aPos, 1.0)).xyz;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
})";