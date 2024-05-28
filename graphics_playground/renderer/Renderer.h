#pragma once
#include <unordered_map>

#include "../components/DirectionLight.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"
#include "../gpu/GPUBackend.h"
#include "../resources/core/Material.h"
#include "../resources/core/Texture.h"
#include "../scene/Camera.h"

#define MAX_POINT_LIGHTS 100
#define MAX_DIRECTION_LIGHTS 100
#define SHADOW_MAP_RESOLUTION 4096
#define MAX_DIRECTION_SHADOWS 10

namespace gp {

/*
todo refs:
https://gamedev.stackexchange.com/questions/73851/how-do-i-fit-the-camera-frustum-inside-directional-light-space
(fit to frustrum)
*/

/*
class RenderCommand {

}

class RenderPass {
  RenderPass(Renderer* renderer)

  virtual void begin(SceneDescription sd);

  virtual void submit(RenderCommand cmd);

  virtual void end();
}

class ShadowPass : public RenderPass {
  ShadowPass(Renderer* renderer) : RenderPass(renderer) {
    // create framebuffers etc etc
  }

  void begin(SceneDescription sd)
}

*/

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
  float size;
  float frustrum_width;
  float padding_3[2];
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
  float near_plane;
};

class Renderer {
 public:
  void resizeViewport(int width, int height, float z_near = 0.1f,
                      float z_far = 100.0f);

  void setClearColour(glm::vec3 col) {
    clear_colour_ = col;
    backend_->setClearColor(clear_colour_.x, clear_colour_.y, clear_colour_.z,
                            1.0f);
  }

  /*TODO: use a World component with ambient light and optionally a texture for
   * IBL*/
  void setAmbientLight(glm::vec3 ambient_light) {
    ambient_light_ = ambient_light;
  }

  void draw(const Camera& camera,
            const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers,
            const std::vector<TransformAnd<PointLight>>& point_lights,
            const std::vector<DirectionLight>& direction_lights);

  Renderer(int scr_width, int scr_height, glm::vec3 clear_color);
  ~Renderer();

 private:
  gpu::ShaderProgramPtr ao_shader_;
  gpu::ShaderProgramPtr blur_shader_;
  gpu::ShaderProgramPtr depth_shader_;
  gpu::ShaderProgramPtr screen_quad_shader_;
  gpu::ShaderProgramPtr shadow_shader_;

  gpu::TexturePtr ao_texture_;
  gpu::TexturePtr blurred_ao_texture_;
  gpu::TexturePtr blurred_colour_texture_;
  gpu::TexturePtr colour_texture_;
  gpu::TexturePtr depth_texture_;
  gpu::TexturePtr normal_texture_;
  gpu::TexturePtr shadow_map_texture_;

  gpu::TexturePtr ssao_noise_texture_;
  std::vector<glm::vec3> ssao_samples_;

  gpu::TexturePtr noise_texture_;
  gpu::TexturePtr disk_samples_texture_;

  gpu::FrameBufferPtr shadow_frame_buffer_;
  gpu::FrameBufferPtr ss_frame_buffer_;
  gpu::FrameBufferPtr colour_frame_buffer_;
  gpu::FrameBufferPtr default_frame_buffer_;

  gpu::UniformBufferPtr camera_uniform_buffer_;
  gpu::UniformBufferPtr lights_uniform_buffer_;

  gpu::BatchPtr quad_batch_;

  gpu::Backend* backend_;

  GPULightBuffer gpu_light_buffer_;
  GPUCameraBuffer gpu_camera_buffer_;

  int scr_width_, scr_height_;
  glm::vec3 clear_colour_;
  float z_near_;
  float z_far_;

  glm::vec3 ambient_light_;

  // Refactor to a GPUCache class?
  gpu::TexturePtr loadGPUTexture(const Texture* texture);
  const gpu::TexturePtr& retrieveGPUTexture(const Texture* texture);
  const gpu::BatchPtr& retrieveMeshGPUBatch(const Mesh* mesh);

  std::unordered_map<unsigned int, gpu::TexturePtr> texture_cache_;
  std::unordered_map<unsigned int, gpu::BatchPtr> batch_cache_;

  void uploadRenderData(
      const Camera& camera,
      const std::vector<TransformAnd<PointLight>>& point_lights,
      const std::vector<DirectionLight>& direction_lights);
  void drawShadowPass(
      const Camera& camera,
      const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers,
      const std::vector<TransformAnd<PointLight>>& point_lights,
      const std::vector<DirectionLight>& direction_lights);
  void drawMainPass(
      const Camera& camera,
      const std::vector<TransformAnd<MeshRenderer>>& mesh_renderers);

  void setShaderInputsForMaterial(const Material& mat,
                                  const gpu::ShaderProgramPtr& shader);

  std::vector<glm::vec3> getSSAOKernel(int num_samples) const;
  std::vector<glm::vec3> getSSAONoise(int num_samples) const;
  std::vector<glm::vec2> uniformDiskSamples(int num_samples) const;
  std::vector<float> uniformLinearSamples(int num_samples) const;

  gpu::BatchPtr allocScreenQuadBatch();
};

}  // namespace gp