#pragma once
#include <glad/glad.h>

#include "../GPUIndexBuffer.h"

namespace gp::gpu {
class GlIndexBuffer : public IndexBuffer {
 public:
  GlIndexBuffer();
  ~GlIndexBuffer();

  void bind() override;
  void uploadData(size_t num_indices, size_t data_size,
                  const void* data) override;

 private:
  GLuint id_;
};
}  // namespace gp::gpu
