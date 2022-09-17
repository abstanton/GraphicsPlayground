#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTang;
layout (location = 3) in vec2 aUV;

out vec2 uv;
out vec3 norm;
out vec3 pos;
out mat3 TBN;

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    vec4 cam_position;
};

uniform mat4 model;

void main()
{
   mat3 modelVector = transpose(inverse(mat3(model)));

   vec3 bitang = normalize(cross(normalize(aNorm), normalize(aTang)));

   vec3 T = normalize(modelVector * aTang);
   vec3 B = normalize(modelVector * bitang);
   vec3 N = normalize(modelVector * aNorm);

   TBN = mat3(T,B,N);

   uv = aUV;  
   norm = normalize(modelVector * aNorm);
   pos = vec3(model * vec4(aPos, 1.0));

   gl_Position = projection * view * model * vec4(aPos, 1.0);
}