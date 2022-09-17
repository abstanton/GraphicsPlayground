#pragma once
const char* def_fs = R"(#version 460
out vec4 FragColor;
            
in vec2 uv;
in vec3 norm;
in vec3 pos;
in mat3 TBN;

#define MAX_TOTAL_POINT_LIGHTS 100
#define MAX_TOTAL_DIRECTION_LIGHTS 10

struct PointLight {
    vec4 position;
    vec4 colour;
    float intensity;
};

struct DirectionLight {
    vec4 direction;
    vec4 colour;
    float intensity;
};

layout(std140, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 projection;
    vec4 cam_position;
};

layout(std140, binding = 1) uniform Lights {
    vec4 ambientLight;
    int numPointLights;
    int numDirectionLights;
    vec3 padding;
    PointLight point_lights[MAX_TOTAL_POINT_LIGHTS];
    DirectionLight direction_lights[MAX_TOTAL_DIRECTION_LIGHTS];
};

uniform vec3 colour;

void main()
{
    vec3 diffuseCol = vec3(0.0,0.0,0.0);
    vec3 specularCol = vec3(0.0,0.0,0.0);    
     
    vec3 normal = normalize(norm);

    vec3 specular_sample = colour;
    vec3 diffuse_sample = vec3(0.5);

     for (int i = 0; i < numPointLights; i++) {
        PointLight light = point_lights[i];
        vec3 position = vec3(light.position.x, light.position.y, light.position.z);
        vec3 colour = vec3(light.colour.x, light.colour.y, light.colour.z);

        vec3 lightDir = normalize(position - pos);
        float diffuse = max(dot(normal, lightDir), 0.0f);
        vec3 diffuseTemp = diffuse * colour * light.intensity;

        vec3 viewDir = normalize(vec3(cam_position) - pos);    
        vec3 reflectDir = reflect(-lightDir, normal);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * light.intensity;
        vec3 specularTemp = specular_sample * spec * colour;  
        diffuseCol += diffuseTemp;
        specularCol += specularTemp;
    }

    for (int i = 0; i < 1; i++) {
        DirectionLight light = direction_lights[i];
        vec3 colour = vec3(light.colour.x, light.colour.y, light.colour.z);

        vec3 lightDir = normalize(vec3(light.direction));
        float diffuse = max(dot(normal, lightDir), 0.0f);
        vec3 diffuseTemp = diffuse * colour * light.intensity;

        vec3 viewDir = normalize(vec3(cam_position) - pos);    
        vec3 reflectDir = reflect(-lightDir, normal);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * light.intensity;
        vec3 specularTemp = specular_sample * spec * colour;  
        diffuseCol += diffuseTemp;
        specularCol += specularTemp;
    }
   
    vec3 result = (vec3(ambientLight) + diffuseCol + specularCol) * colour;
    FragColor = vec4(result, 1.0f);
})";