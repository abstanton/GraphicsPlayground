#include "Renderer.h"

#include "../core/managers/ShaderManager.h"

Renderer::Renderer(int scr_width, int scr_height, glm::vec3 clear_colour)
    : scr_width_(scr_width),
      scr_height_(scr_height),
      clear_colour(clear_colour),
      gpu_light_buffer_({}),
      gpu_camera_buffer_({}) {
  backend_ = gpu::Backend::get();

  auto& shader_manager = ShaderManager::get();
  shadow_shader_ = shader_manager.getShader("shadow");

  camera_uniform_buffer_ =
      backend_->allocUniformBuffer(sizeof(GPUCameraBuffer));
  lights_uniform_buffer_ = backend_->allocUniformBuffer(sizeof(GPULightBuffer));

  glClearColor(clear_colour.x, clear_colour.y, clear_colour.z, 1.0f);

  shadow_frame_buffer_ = backend_->allocFrameBuffer();
  shadow_map_texture_ = backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::DEPTH,
      gpu::DataType::FLOAT, SHADOW_MAP_RESOLUTION * MAX_DIRECTION_SHADOWS,
      SHADOW_MAP_RESOLUTION, 0, 0, 0, nullptr);
  shadow_frame_buffer_->attachTexture(
      shadow_map_texture_, gpu::TextureAttachmentType::DepthAttachment, 0, 0);
}

Renderer::~Renderer() {
  batch_cache_.clear();
  texture_cache_.clear();

  delete shadow_map_texture_;
  delete shadow_frame_buffer_;
  delete lights_uniform_buffer_;
  delete camera_uniform_buffer_;
}

void Renderer::draw(Camera camera, std::vector<MeshRenderer> mesh_renderers,
                    std::vector<PointLight> point_lights,
                    std::vector<DirectionLight> direction_lights) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  begin(camera, point_lights, direction_lights);
  drawShadowPass(mesh_renderers, point_lights, direction_lights);
  drawMainPass(mesh_renderers);
}

void Renderer::begin(Camera camera, std::vector<PointLight> point_lights,
                     std::vector<DirectionLight> direction_lights) {
  glm::vec3 ambient_light = clear_colour;

  glm::mat4 view_matrix = camera.getViewMatrix();
  glm::mat4 projection_matrix =
      glm::perspective(glm::radians(camera.Zoom),
                       (float)scr_width_ / (float)scr_height_, 0.1f, 100.0f);

  gpu_camera_buffer_.view = view_matrix;
  gpu_camera_buffer_.projection = projection_matrix;
  gpu_camera_buffer_.position = camera.Position;

  gpu_light_buffer_.ambient_light = ambient_light;
  gpu_light_buffer_.num_point_lights = static_cast<int>(point_lights.size());
  gpu_light_buffer_.num_direction_lights =
      static_cast<int>(direction_lights.size());
  for (int i = 0; i < point_lights.size(); i++) {
    GPUPointLight point_light;
    point_light.intensity = point_lights[i].intensity;
    point_light.colour = point_lights[i].colour;
    point_light.position = point_lights[i].position;
    gpu_light_buffer_.point_lights[i] = point_light;
  }
  for (int i = 0; i < direction_lights.size(); i++) {
    GPUDirectionLight direction_light;
    direction_light.intensity = direction_lights[i].intensity;
    direction_light.is_shadowed = 1;
    direction_light.colour = direction_lights[i].colour;
    direction_light.direction = direction_lights[i].direction;
    float near_plane = 1.0f, far_plane = 10.0f;
    glm::mat4 light_projection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 light_view =
        glm::lookAt(direction_light.direction, glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 light_space_matrix = light_projection * light_view;
    direction_light.light_space_matrix = light_space_matrix;
    gpu_light_buffer_.direction_lights[i] = direction_light;
  }

  lights_uniform_buffer_->uploadData(&gpu_light_buffer_);
  camera_uniform_buffer_->uploadData(&gpu_camera_buffer_);
}

void Renderer::drawShadowPass(std::vector<MeshRenderer> mesh_renderers,
                              std::vector<PointLight> point_lights,
                              std::vector<DirectionLight> direction_lights) {
  shadow_frame_buffer_->bind();
  glClear(GL_DEPTH_BUFFER_BIT);
  shadow_shader_->use();

  for (int i = 0; i < direction_lights.size(); i++) {
    backend_->setViewport(i * SHADOW_MAP_RESOLUTION, 0, SHADOW_MAP_RESOLUTION,
                          SHADOW_MAP_RESOLUTION);
    float near_plane = 1.0f, far_plane = 10.0f;
    glm::mat4 lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView =
        glm::lookAt(direction_lights[i].direction, glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    shadow_shader_->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    for (auto mc : mesh_renderers) {
      shadow_shader_->setMat4("model", mc.transform_.getTransformation());

      gpu::Batch* batch = retrieveMeshGPUBatch(mc.mesh_);
      batch->draw();
    }
  }
  shadow_frame_buffer_->unbind();
  backend_->setViewport(0, 0, scr_width_, scr_height_);
}

void Renderer::drawMainPass(std::vector<MeshRenderer> mesh_renderers) {
  camera_uniform_buffer_->bind(0);
  lights_uniform_buffer_->bind(1);
  for (auto mc : mesh_renderers) {
    mc.material_->shader_->use();

    const Material* material = mc.material_.get();
    auto textures = material->getTextures();
    for (int i = 0; i < textures.size(); i++) {
      gpu::Texture* gpu_texture = retrieveGPUTexture(textures[i]);
      gpu_texture->bind(i);
    }
    shadow_map_texture_->bind(10);

    // TODO don't do this
    mc.material_->shader_->setMat4("model", mc.transform_.getTransformation());
    mc.material_->shader_->setFloat("tex_scale", mc.material_->tex_scale);

    gpu::Batch* batch = retrieveMeshGPUBatch(mc.mesh_);

    batch->draw();
  }
}

gpu::Texture* Renderer::retrieveGPUTexture(const Texture* texture) {
  if (texture_cache_.find(texture->id) != texture_cache_.end()) {
    return texture_cache_[texture->id];
  }

  gpu::TextureFormat gpu_texture_format;

  switch (texture->format) {
    case TextureFormat::R8:
      gpu_texture_format = gpu::TextureFormat::R;
      break;
    case TextureFormat::RGB8:
      gpu_texture_format = gpu::TextureFormat::RGB;
      break;
    case TextureFormat::RGBA8:
      gpu_texture_format = gpu::TextureFormat::RGBA;
      break;
    default:
      gpu_texture_format = gpu::TextureFormat::RGB;
  }

  gpu::Texture* gpu_texture = backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu_texture_format,
      gpu::DataType::UNSIGNED_BYTE, texture->width, texture->height, 0, 0, 0,
      texture->data);
  gpu_texture->generateMipmap();

  texture_cache_[texture->id] = gpu_texture;
  return gpu_texture;
}

gpu::Batch* Renderer::retrieveMeshGPUBatch(const Mesh* mesh) {
  if (batch_cache_.find(mesh->id) != batch_cache_.end()) {
    return batch_cache_[mesh->id];
  }

  gpu::VertexBuffer* vertex_buffer = backend_->allocVertexBuffer();
  gpu::IndexBuffer* index_buffer = backend_->allocIndexBuffer();

  std::vector<gpu::VertexAttrib> vertex_attribs = {
      {"pos", 0, 3, gpu::DataType::FLOAT,
       false,  // TODO: What does this do?
       sizeof(Vertex), (void*)0},
      {"norm", 1, 3, gpu::DataType::FLOAT,
       false,  // TODO: What does this do?
       sizeof(Vertex), (void*)(offsetof(Vertex, norm))},
      {"tang", 2, 3, gpu::DataType::FLOAT,
       false,  // TODO: What does this do?
       sizeof(Vertex), (void*)(offsetof(Vertex, tang))},
      {"uv", 3, 2, gpu::DataType::FLOAT,
       false,  // TODO: What does this do?
       sizeof(Vertex), (void*)(offsetof(Vertex, uv))}};

  vertex_buffer->uploadData(vertex_attribs, mesh->verts_.size(),
                            sizeof(Vertex) * mesh->verts_.size(),
                            mesh->verts_.data());
  index_buffer->uploadData(mesh->indices_.size(),
                           mesh->indices_.size() * sizeof(unsigned int),
                           mesh->indices_.data());

  gpu::Batch* batch = backend_->allocBatch(vertex_buffer, index_buffer);
  batch_cache_[mesh->id] = batch;

  return batch;
}
