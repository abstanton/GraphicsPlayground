#pragma once
#include <string>

#include "GPUBatch.h"
#include "GPUFrameBuffer.h"
#include "GPUIndexBuffer.h"
#include "GPUShader.h"
#include "GPUTexture.h"
#include "GPUUniformBuffer.h"
#include "GPUVertexBuffer.h"

namespace gpu {

enum class ClearType { ALL, DEPTH, COLOR };

class Backend {
 public:
  static Backend* get();

  virtual Texture* generateTexture(TextureType type, TextureFormat format,
                                   DataType data_type, unsigned int width,
                                   unsigned int height, unsigned int depth,
                                   unsigned int mips, unsigned int levels,
                                   const void* data) = 0;

  virtual FrameBuffer* defaultFrameBuffer() = 0;
  virtual FrameBuffer* allocFrameBuffer() = 0;
  virtual UniformBuffer* allocUniformBuffer(size_t size) = 0;
  virtual IndexBuffer* allocIndexBuffer() = 0;
  virtual VertexBuffer* allocVertexBuffer() = 0;

  virtual Shader* compileShaderProgram(const char* vs_source,
                                       const char* fs_source) = 0;

  virtual Batch* allocBatch(VertexBuffer* vert_buff) = 0;
  virtual Batch* allocBatch(VertexBuffer* vert_buff,
                            IndexBuffer* index_buff) = 0;

  virtual void setViewport(int x, int y, int width, int height) = 0;
  virtual void unbindFramebuffer() = 0;

  virtual void clear(ClearType) = 0;
  virtual void setClearColor(float r, float g, float b, float a) = 0;
};
}  // namespace gpu
