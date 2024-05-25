#pragma once
#include "../GPUBackend.h"
#include "GlBatch.h"
#include "GlFrameBuffer.h"
#include "GlIndexBuffer.h"
#include "GlShaderProgram.h"
#include "GlTexture.h"
#include "GlUniformBuffer.h"
#include "GlVertexArray.h"
#include "GlVertexBuffer.h"

namespace gp::gpu {
class GlBackend : public Backend {
 public:
  GlBackend();

  FrameBufferPtr defaultFrameBuffer() override;
  UniformBufferPtr allocUniformBuffer(size_t size) override;
  FrameBufferPtr allocFrameBuffer() override;
  IndexBufferPtr allocIndexBuffer() override;
  VertexBufferPtr allocVertexBuffer() override;

  BatchPtr allocBatch(VertexBufferPtr vert_buff) override;
  BatchPtr allocBatch(VertexBufferPtr vert_buff,
                      IndexBufferPtr index_buff) override;

  ShaderProgramPtr allocShaderProgram() override;

  TexturePtr generateTexture(
      TextureType type, TextureFormat format, DataType data_type,
      unsigned int width, unsigned int height, unsigned int depth,
      unsigned int mips, unsigned int levels, const void* data,
      TextureFilter min_filter = TextureFilter::NEAREST,
      TextureFilter mag_filter = TextureFilter::NEAREST,
      TextureWrapping wrap_s = TextureWrapping::REPEAT,
      TextureWrapping wrap_t = TextureWrapping::REPEAT) override;

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
}  // namespace gp::gpu
