#pragma once
#include <glad/glad.h>

#include "../GPUBatch.h"

namespace gpu {
class GlBatch : public Batch {
 public:
  GlBatch(VertexBuffer* vert_buff);
  GlBatch(VertexBuffer* vert_buff, IndexBuffer* index_buff);
  ~GlBatch();

  void draw() const override;
  void drawIndirect() const override;

 private:
  void vboBind();

  GLuint vao_id_;
};
}  // namespace gpu
