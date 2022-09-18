#pragma once
#include "../GPUBackend.h"
#include "GlBatch.h"
#include "GlFrameBuffer.h"
#include "GlIndexBuffer.h"
#include "GlShader.h"
#include "GlTexture.h"
#include "GlUniformBuffer.h"
#include "GlVertexArray.h"
#include "GlVertexBuffer.h"

namespace gpu {
class GlBackend : public Backend {
 public:
  GlBackend();

  UniformBuffer* allocUniformBuffer(size_t size) override;
  FrameBuffer* allocFrameBuffer() override;
  IndexBuffer* allocIndexBuffer() override;
  VertexBuffer* allocVertexBuffer() override;

  Batch* allocBatch(VertexBuffer* vert_buff) override;
  Batch* allocBatch(VertexBuffer* vert_buff, IndexBuffer* index_buff) override;

  Shader* compileShaderProgram(const char* vs_source,
                               const char* fs_source) override;

  Texture* generateTexture(TextureType type, TextureFormat format,
                           DataType data_type, unsigned int width,
                           unsigned int height, unsigned int depth,
                           unsigned int mips, unsigned int levels,
                           const void* data) override;

  void setViewport(int x, int y, int width, int height) override;
  void unbindFramebuffer() override;

  void clear(ClearType type) override {
    switch (type) {
      case ClearType::COLOR:
        glClear(GL_COLOR_BUFFER_BIT);
        break;
      case ClearType::DEPTH:
        glClear(GL_DEPTH_BUFFER_BIT);
        break;
      default:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        break;
    }
  }

  void setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
  }
};
}  // namespace gpu
