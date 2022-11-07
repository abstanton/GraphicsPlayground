#include "Renderer.h"

#include <iostream>
#include <random>

#include "../resources/ResourceManager.h"
#include "../shaders/ShaderManager.h"

Renderer::Renderer(int scr_width, int scr_height, glm::vec3 clear_colour)
    : scr_width_(scr_width),
      scr_height_(scr_height),
      clear_colour(clear_colour),
      gpu_light_buffer_({}),
      gpu_camera_buffer_({}) {
  backend_ = gpu::Backend::get();
  backend_->setClearColor(clear_colour.x, clear_colour.y, clear_colour.z, 1.0f);

  ao_shader_ = ShaderManager::get().getShader("ssao");
  blur_shader_ = ShaderManager::get().getShader("blur");
  depth_shader_ = ShaderManager::get().getShader("depth");
  screen_quad_shader_ = ShaderManager::get().getShader("post");
  shadow_shader_ = ShaderManager::get().getShader("shadow");

  camera_uniform_buffer_.reset(
      backend_->allocUniformBuffer(sizeof(GPUCameraBuffer)));
  lights_uniform_buffer_.reset(
      backend_->allocUniformBuffer(sizeof(GPULightBuffer)));

  shadow_frame_buffer_.reset(backend_->allocFrameBuffer());
  ss_frame_buffer_.reset(backend_->allocFrameBuffer());
  colour_frame_buffer_.reset(backend_->allocFrameBuffer());
  default_frame_buffer_.reset(backend_->defaultFrameBuffer());

  auto ssao_noise = getSSAONoise(16);
  ssao_noise_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::RGB_16F,
      gpu::DataType::FLOAT, 4, 4, 1, 1, 1, ssao_noise.data()));
  ssao_samples_ = getSSAOKernel(64);

  quad_batch_.reset(allocScreenQuadBatch());

  this->resizeViewport(scr_width, scr_height);
}

void Renderer::resizeViewport(int width, int height, float z_near,
                              float z_far) {
  std::cout << "Resizing viewport" << std::endl;
  backend_->setViewport(0, 0, width, height);

  scr_width_ = width;
  scr_height_ = height;
  z_near_ = z_near;
  z_far_ = z_far;

  shadow_map_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D_ARRAY, gpu::TextureFormat::DEPTH_32,
      gpu::DataType::FLOAT, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 1, 1,
      MAX_DIRECTION_SHADOWS, nullptr, gpu::TextureFilter::NEAREST,
      gpu::TextureFilter::NEAREST, gpu::TextureWrapping::CLAMP_TO_BORDER,
      gpu::TextureWrapping::CLAMP_TO_BORDER));
  ao_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::R_32F,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr,
      gpu::TextureFilter::NEAREST, gpu::TextureFilter::NEAREST,
      gpu::TextureWrapping::CLAMP_TO_BORDER,
      gpu::TextureWrapping::CLAMP_TO_BORDER));
  blurred_ao_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::R_32F,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr,
      gpu::TextureFilter::NEAREST, gpu::TextureFilter::NEAREST,
      gpu::TextureWrapping::CLAMP_TO_BORDER,
      gpu::TextureWrapping::CLAMP_TO_BORDER));
  blurred_colour_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::RGBA_16F,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr));
  depth_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::DEPTH_32,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr,
      gpu::TextureFilter::NEAREST, gpu::TextureFilter::NEAREST,
      gpu::TextureWrapping::CLAMP_TO_BORDER,
      gpu::TextureWrapping::CLAMP_TO_BORDER));
  normal_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::RGB_16F,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr,
      gpu::TextureFilter::NEAREST, gpu::TextureFilter::NEAREST,
      gpu::TextureWrapping::CLAMP_TO_BORDER,
      gpu::TextureWrapping::CLAMP_TO_BORDER));
  colour_texture_.reset(backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu::TextureFormat::RGBA_16F,
      gpu::DataType::FLOAT, scr_width_, scr_height_, 1, 1, 1, nullptr));

  colour_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::DepthAttachment,
      {depth_texture_.get(), 0, 0});
  colour_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::ColorAttachment0,
      {colour_texture_.get(), 0, 0});
  colour_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::ColorAttachment1,
      {normal_texture_.get(), 0, 0});
  ss_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::ColorAttachment0,
      {ao_texture_.get(), 0, 0});
}

Renderer::~Renderer() {
  for (auto [id, resource] : batch_cache_) {
    delete resource;
  }
  for (auto [id, texture] : texture_cache_) {
    delete texture;
  }
  batch_cache_.clear();
  texture_cache_.clear();
}

void Renderer::draw(Camera camera, std::vector<MeshPair> mesh_pairs,
                    std::vector<PointLight> point_lights,
                    std::vector<DirectionLight> direction_lights) {
  backend_->clear(gpu::ClearType::ALL);
  uploadRenderData(camera, point_lights, direction_lights);
  drawShadowPass(mesh_pairs, point_lights, direction_lights);
  drawMainPass(mesh_pairs);
}

void Renderer::uploadRenderData(Camera camera,
                                std::vector<PointLight> point_lights,
                                std::vector<DirectionLight> direction_lights) {
  glm::vec3 ambient_light = clear_colour;

  glm::mat4 view_matrix = camera.getViewMatrix();
  glm::mat4 projection_matrix =
      glm::perspective(glm::radians(camera.Zoom),
                       (float)scr_width_ / (float)scr_height_, z_near_, z_far_);

  gpu_camera_buffer_.view = view_matrix;
  gpu_camera_buffer_.projection = projection_matrix;
  gpu_camera_buffer_.position = camera.Position;
  gpu_camera_buffer_.inverse_proj = glm::inverse(projection_matrix);

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

    // TODO: Get this out of here!
    float near_plane = 1.0f, far_plane = 20.0f;
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

void Renderer::drawShadowPass(std::vector<MeshPair> mesh_renderers,
                              std::vector<PointLight> point_lights,
                              std::vector<DirectionLight> direction_lights) {
  shadow_frame_buffer_->bind();
  backend_->setViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
  shadow_shader_->use();
  glCullFace(GL_FRONT);

  // TODO: Investigate how to abstract this
  glDepthMask(GL_TRUE);
  glColorMask(0, 0, 0, 0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  for (int i = 0; i < direction_lights.size(); i++) {
    if (i >= MAX_DIRECTION_SHADOWS) {
      break;
    }
    // Attach appropriate layer of depth texture array
    shadow_frame_buffer_->addAttachment(
        gpu::FrameBufferAttachmentType::DepthAttachment,
        {shadow_map_texture_.get(), i, 0});
    shadow_frame_buffer_->bind();
    // clear texture before drawing depth
    backend_->clear(gpu::ClearType::DEPTH);

    float near_plane = 1.0f, far_plane = 20.0f;
    glm::mat4 lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView =
        glm::lookAt(direction_lights[i].direction, glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    shadow_shader_->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    for (auto&& [mc, transform] : mesh_renderers) {
      shadow_shader_->setMat4("model", transform.transformation());

      gpu::Batch* batch = retrieveMeshGPUBatch(
          ResourceManager::get().getMesh(mc.mesh_).value());
      batch->draw();
    }
  }
  glDisable(GL_POLYGON_OFFSET_FILL);
  glCullFace(GL_BACK);
}

void Renderer::drawMainPass(std::vector<MeshPair> mesh_renderers) {
  colour_frame_buffer_->bind();
  backend_->setViewport(0, 0, scr_width_, scr_height_);

  // -------- DEPTH PRE-PASS ----------
  depth_shader_->use();
  camera_uniform_buffer_->bind(0);
  lights_uniform_buffer_->bind(1);

  // TODO: Refactor this
  glDepthMask(GL_TRUE);
  glColorMask(0, 0, 0, 0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  backend_->clear(gpu::ClearType::ALL);

  for (auto&& [mc, transform] : mesh_renderers) {
    depth_shader_->setMat4("model", transform.transformation());

    gpu::Batch* batch =
        retrieveMeshGPUBatch(ResourceManager::get().getMesh(mc.mesh_).value());

    batch->draw();
  }

  // -------- FORWARD PASS ----------

  // TODO: Refactor this
  glDepthMask(GL_FALSE);
  glColorMask(1, 1, 1, 1);
  glDepthFunc(GL_LEQUAL);

  backend_->clear(gpu::ClearType::COLOR);
  for (auto&& [mc, transform] : mesh_renderers) {
    gpu::ShaderProgram* shader =
        ShaderManager::get().getShader(mc.material_comp_.shader_name);
    shader->use();
    setShaderInputsForMaterial(mc.material_comp_, shader);
    shadow_map_texture_->bind(10);
    depth_texture_->bind(11);
    shader->setMat4("model", transform.transformation());

    gpu::Batch* batch =
        retrieveMeshGPUBatch(ResourceManager::get().getMesh(mc.mesh_).value());

    batch->draw();
  }

  // -------- Screen Space Effects ---------
  // -------- SSAO -----------
  ss_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::ColorAttachment0,
      {ao_texture_.get(), 0, 0});
  ss_frame_buffer_->bind();
  ao_shader_->use();
  ao_shader_->setVec3Arr("samples", ssao_samples_.data(), 64);
  depth_texture_->bind(0);
  normal_texture_->bind(1);
  ssao_noise_texture_->bind(5);
  quad_batch_->draw();
  // // blur
  ss_frame_buffer_->addAttachment(
      gpu::FrameBufferAttachmentType::ColorAttachment0,
      {blurred_ao_texture_.get(), 0, 0});
  ss_frame_buffer_->bind();
  blur_shader_->use();
  ao_texture_->bind(0);
  blur_shader_->setFloat("radius", 1.0f);
  // Do this with texture or ubo

  quad_batch_->draw();

  // -------- POST PASS ----------
  default_frame_buffer_->bind();
  screen_quad_shader_->use();
  camera_uniform_buffer_->bind(0);
  colour_texture_->bind(0);
  blurred_ao_texture_->bind(1);
  // TODO: Set exposure from camera exposure
  quad_batch_->draw();
  glGetError();
}

gpu::Batch* Renderer::allocScreenQuadBatch() {
  float quadVertices[] = {// positions   // texCoords
                          -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  std::vector<gpu::VertexAttrib> attribs = {
      {"pos", 0, 2, gpu::DataType::FLOAT, false, 4 * sizeof(float), (void*)0},
      {"uv", 1, 2, gpu::DataType::FLOAT, false, 4 * sizeof(float),
       (void*)(2 * sizeof(float))}};
  gpu::VertexBuffer* vert_buffer = backend_->allocVertexBuffer();
  vert_buffer->uploadData(attribs, 6, sizeof(quadVertices), quadVertices);
  return backend_->allocBatch(vert_buffer);
}

gpu::Texture* Renderer::retrieveGPUTexture(const Texture* texture) {
  if (texture_cache_.find(texture->id) != texture_cache_.end()) {
    return texture_cache_[texture->id];
  }

  gpu::TextureFormat gpu_texture_format;
  gpu::DataType gpu_data_type;

  switch (texture->format) {
    case TextureFormat::R:
      gpu_texture_format = gpu::TextureFormat::R;
      gpu_data_type = gpu::DataType::UNSIGNED_BYTE;
      break;
    case TextureFormat::R8:
      gpu_texture_format = gpu::TextureFormat::R;
      gpu_data_type = gpu::DataType::UNSIGNED_BYTE;
      break;
    case TextureFormat::R16:
      gpu_texture_format = gpu::TextureFormat::R_16F;
      gpu_data_type = gpu::DataType::FLOAT;
      break;
    case TextureFormat::R32:
      gpu_texture_format = gpu::TextureFormat::R_32F;
      gpu_data_type = gpu::DataType::FLOAT;
      break;
    case TextureFormat::RGB8:
      gpu_texture_format = gpu::TextureFormat::RGB;
      gpu_data_type = gpu::DataType::UNSIGNED_BYTE;
      break;
    case TextureFormat::RGBA8:
      gpu_texture_format = gpu::TextureFormat::RGBA;
      gpu_data_type = gpu::DataType::UNSIGNED_BYTE;
      break;
    default:
      gpu_texture_format = gpu::TextureFormat::RGB;
      gpu_data_type = gpu::DataType::UNSIGNED_BYTE;
  }

  gpu::Texture* gpu_texture = backend_->generateTexture(
      gpu::TextureType::TEXTURE_2D, gpu_texture_format, gpu_data_type,
      texture->width, texture->height, 0, 0, 0, texture->data,
      gpu::TextureFilter::LINEAR);
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
      {"pos", 0, 3, gpu::DataType::FLOAT, false, sizeof(Vertex), (void*)0},
      {"norm", 1, 3, gpu::DataType::FLOAT, false, sizeof(Vertex),
       (void*)(offsetof(Vertex, norm))},
      {"tang", 2, 3, gpu::DataType::FLOAT, false, sizeof(Vertex),
       (void*)(offsetof(Vertex, tang))},
      {"uv", 3, 2, gpu::DataType::FLOAT, false, sizeof(Vertex),
       (void*)(offsetof(Vertex, uv))}};

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

void Renderer::setShaderInputsForMaterial(const Material& mat,
                                          gpu::ShaderProgram* shader) {
  for (auto&& [name, input] : mat.colour_inputs) {
    if (input.use_tex) {
      shader->setBool(name + "_use_tex", true);
      shader->setVec2(name + "_scale", input.scale);
      shader->setTexture(
          (name + "_tex").c_str(),
          retrieveGPUTexture(
              ResourceManager::get().getTexture(input.tex_name).value()));
    } else {
      shader->setBool(name + "_use_tex", false);
      shader->setVec3(name + "_val", input.value);
    }
  }
  for (auto&& [name, input] : mat.float_inputs) {
    if (input.use_tex) {
      shader->setBool(name + "_use_tex", true);
      shader->setVec2(name + "_scale", input.scale);
      shader->setTexture(
          (name + "_tex").c_str(),
          retrieveGPUTexture(
              ResourceManager::get().getTexture(input.tex_name).value()));
    } else {
      shader->setBool(name + "_use_tex", false);
      shader->setFloat(name + "_val", input.value);
    }
  }
  for (auto&& [name, input] : mat.float_uniforms) {
    shader->setFloat(name, input.value);
  }
  for (auto&& [name, input] : mat.colour_uniforms) {
    shader->setVec3(name, input.value);
  }
  for (auto&& [name, input] : mat.bool_uniforms) {
    shader->setBool(name, input.value);
  }
}

float lerp(float a, float b, float f) { return a + f * (b - a); }

std::vector<glm::vec3> Renderer::getSSAOKernel(int num_samples) const {
  std::uniform_real_distribution<float> random_floats(
      0.0, 1.0);  // random floats between [0.0, 1.0]
  std::default_random_engine generator;
  std::vector<glm::vec3> ssao_kernel;
  for (int i = 0; i < num_samples; ++i) {
    glm::vec3 sample(random_floats(generator) * 2.0 - 1.0,
                     random_floats(generator) * 2.0 - 1.0,
                     random_floats(generator));
    sample = glm::normalize(sample);
    sample *= random_floats(generator);
    float scale = (float)i / 64.0f;
    scale = lerp(0.1f, 1.0f, scale * scale);
    sample *= scale;
    ssao_kernel.push_back(sample);
  }
  return ssao_kernel;
}

std::vector<glm::vec3> Renderer::getSSAONoise(int num_samples) const {
  std::uniform_real_distribution<float> random_floats(
      0.0, 1.0);  // random floats between [0.0, 1.0]
  std::default_random_engine generator;
  std::vector<glm::vec3> ssao_noise;
  for (int i = 0; i < 16; i++) {
    glm::vec3 noise(random_floats(generator) * 2.0 - 1.0,
                    random_floats(generator) * 2.0 - 1.0, 0.0f);
    ssao_noise.push_back(glm::normalize(noise));
  }
  return ssao_noise;
}
