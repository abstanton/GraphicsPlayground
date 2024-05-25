#pragma once
#include <glad/glad.h>

#include "../GPUVertexBuffer.h"
namespace gp::gpu {
class GlVertexBuffer : public VertexBuffer {
 public:
  GlVertexBuffer();
  ~GlVertexBuffer();

  void bind() override;
  void uploadData(std::vector<VertexAttrib> vertex_attribs, size_t num_verts,
                  size_t data_size, const void* data) override;

 private:
  GLuint id_;
};
}  // namespace gp::gpu
