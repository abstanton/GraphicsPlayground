#pragma once
#include <unordered_map>

#include "../Material.h"
#include "../components/Camera.h"
#include "../components/DirectionLight.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"
#include "../gpu/GPUBackend.h"

#define MAX_POINT_LIGHTS 100
#define MAX_DIRECTION_LIGHTS 100
#define SHADOW_MAP_RESOLUTION 1024
#define MAX_DIRECTION_SHADOWS 10

enum class RenderPass { SHADOW_MAP, DRAW };

struct GPUPointLight {
  glm::vec3 position;
  float padding_1;
  glm::vec3 colour;
  float padding_2;
  float intensity;
  float padding_0[3];
};

struct GPUDirectionLight {
  glm::vec3 direction;
  float padding_1;
  glm::vec3 colour;
  float padding_2;
  float intensity;
  int is_shadowed;
  float padding_0[2];
  glm::mat4 light_space_matrix;
};

struct GPULightBuffer {
  glm::vec3 ambient_light;
  float padding_0;
  int num_point_lights;
  int num_direction_lights;
  float padding_1[2];
  GPUPointLight point_lights[MAX_POINT_LIGHTS];
  GPUDirectionLight direction_lights[MAX_DIRECTION_LIGHTS];
};

struct GPUCameraBuffer {
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  float padding;
};

class Renderer {
 public:
  void draw(Camera camera, std::vector<MeshRenderer> mesh_renderers,
            std::vector<PointLight> point_lights,
            std::vector<DirectionLight> direction_lights);

  Renderer(int scr_width, int scr_height, glm::vec3 clear_color);
  ~Renderer();

 private:
  gpu::Shader* shadow_shader_;

  GPULightBuffer gpu_light_buffer_;
  GPUCameraBuffer gpu_camera_buffer_;

  int scr_width_, scr_height_;
  glm::vec3 clear_colour;

  gpu::Texture* retrieveGPUTexture(const Texture* texture);
  gpu::Batch* retrieveMeshGPUBatch(const Mesh* mesh);

  gpu::Backend* backend_;
  gpu::Texture* shadow_map_texture_;
  gpu::FrameBuffer* shadow_frame_buffer_;

  gpu::UniformBuffer* lights_uniform_buffer_;
  gpu::UniformBuffer* camera_uniform_buffer_;

  std::unordered_map<unsigned int, gpu::Texture*> texture_cache_;
  std::unordered_map<unsigned int, gpu::Batch*> batch_cache_;

  void begin(Camera camera, std::vector<PointLight> point_lights,
             std::vector<DirectionLight> direction_lights);
  void drawShadowPass(std::vector<MeshRenderer> mesh_renderers,
                      std::vector<PointLight> point_lights,
                      std::vector<DirectionLight> direction_lights);
  void drawMainPass(std::vector<MeshRenderer> mesh_renderers);

  void setShaderInputsForMaterial(const Material& mat, gpu::Shader* shader);
};
