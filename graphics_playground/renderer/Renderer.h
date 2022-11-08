#pragma once
#include <unordered_map>

#include "../components/Camera.h"
#include "../components/DirectionLight.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"
#include "../gpu/GPUBackend.h"
#include "../resources/core/Material.h"
#include "../resources/core/Texture.h"

#define MAX_POINT_LIGHTS 100
#define MAX_DIRECTION_LIGHTS 100
#define SHADOW_MAP_RESOLUTION 1024
#define MAX_DIRECTION_SHADOWS 10

template <typename T>
struct TransformAnd {
  T data;
  Transform transform;
};

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
  glm::mat4 inverse_proj;
  glm::vec3 position;
  float padding;
};

class Renderer {
 public:
  void resizeViewport(int width, int height, float z_near = 0.1f,
                      float z_far = 100.0f);

  void setClearColour(glm::vec3 col) {
    clear_colour = col;
    backend_->setClearColor(clear_colour.x, clear_colour.y, clear_colour.z,
                            1.0f);
  }

  void draw(const Camera& camera,
            const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers,
            const std::vector<TransformAnd<PointLight>>& point_lights,
            const std::vector<DirectionLight>& direction_lights);

  Renderer(int scr_width, int scr_height, glm::vec3 clear_color);
  ~Renderer();

 private:
  gpu::ShaderProgram* ao_shader_;
  gpu::ShaderProgram* blur_shader_;
  gpu::ShaderProgram* depth_shader_;
  gpu::ShaderProgram* screen_quad_shader_;
  gpu::ShaderProgram* shadow_shader_;

  std::unique_ptr<gpu::Texture> ao_texture_;
  std::unique_ptr<gpu::Texture> blurred_ao_texture_;
  std::unique_ptr<gpu::Texture> blurred_colour_texture_;
  std::unique_ptr<gpu::Texture> colour_texture_;
  std::unique_ptr<gpu::Texture> depth_texture_;
  std::unique_ptr<gpu::Texture> normal_texture_;
  std::unique_ptr<gpu::Texture> shadow_map_texture_;

  std::unique_ptr<gpu::Texture> ssao_noise_texture_;
  std::vector<glm::vec3> ssao_samples_;

  std::unique_ptr<gpu::FrameBuffer> shadow_frame_buffer_;
  std::unique_ptr<gpu::FrameBuffer> ss_frame_buffer_;
  std::unique_ptr<gpu::FrameBuffer> colour_frame_buffer_;
  std::unique_ptr<gpu::FrameBuffer> default_frame_buffer_;

  std::unique_ptr<gpu::UniformBuffer> camera_uniform_buffer_;
  std::unique_ptr<gpu::UniformBuffer> lights_uniform_buffer_;

  std::unique_ptr<gpu::Batch> quad_batch_;

  gpu::Backend* backend_;

  GPULightBuffer gpu_light_buffer_;
  GPUCameraBuffer gpu_camera_buffer_;

  int scr_width_, scr_height_;
  glm::vec3 clear_colour;
  float z_near_;
  float z_far_;

  const std::unique_ptr<gpu::Texture>& retrieveGPUTexture(const Texture* texture);
  const std::unique_ptr<gpu::Batch>& retrieveMeshGPUBatch(const Mesh* mesh);

  std::unordered_map<unsigned int, std::unique_ptr<gpu::Texture>> texture_cache_;
  std::unordered_map<unsigned int, std::unique_ptr<gpu::Batch>> batch_cache_;

  void uploadRenderData(
      const Camera& camera,
      const std::vector<TransformAnd<PointLight>>& point_lights,
      const std::vector<DirectionLight>& direction_lights);
  void drawShadowPass(
      const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers,
      const std::vector<TransformAnd<PointLight>>& point_lights,
      const std::vector<DirectionLight>& direction_lights);
  void drawMainPass(
      const Camera& camera,
      const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers);

  void setShaderInputsForMaterial(const Material& mat,
                                  gpu::ShaderProgram* shader);

  std::vector<glm::vec3> getSSAOKernel(int num_samples) const;
  std::vector<glm::vec3> getSSAONoise(int num_samples) const;

  gpu::Batch* allocScreenQuadBatch();
};
