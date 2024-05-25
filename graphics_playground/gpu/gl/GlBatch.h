#pragma once
#include <glad/glad.h>

#include "../GPUBatch.h"

namespace gp::gpu {
class GlBatch : public Batch {
 public:
  GlBatch(VertexBufferPtr vert_buff);
  GlBatch(VertexBufferPtr vert_buff, IndexBufferPtr index_buff);
  ~GlBatch() {}

  void draw() const override;
  void drawIndirect() const override;

 private:
  void vboBind();

  GLuint vao_id_;
};
}  // namespace gp::gpu
