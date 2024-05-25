#pragma once
#include <glad/glad.h>

#include "../GPUUniformBuffer.h"

namespace gp::gpu {
class GlUniformBuffer : public UniformBuffer {
 public:
  GlUniformBuffer(size_t size);
  ~GlUniformBuffer();
  void uploadData(void* data) override;
  void bind() override;
  void bind(int slot) override;

 private:
  GLuint id_;
};
}  // namespace gp::gpu
