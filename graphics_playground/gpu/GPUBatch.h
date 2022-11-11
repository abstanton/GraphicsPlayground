#pragma once
#include <memory>

#include "GPUIndexBuffer.h"
#include "GPUVertexBuffer.h"

namespace gpu {

class Batch {
  using BatchPtr = std::shared_ptr<Batch>;

 protected:
  VertexBuffer* vert_buff_ = nullptr;
  IndexBuffer* index_buff_ = nullptr;

 public:
  Batch(VertexBuffer* vert_buff) : vert_buff_(vert_buff) {}
  Batch(VertexBuffer* vert_buff, IndexBuffer* index_buff)
      : vert_buff_(vert_buff), index_buff_(index_buff) {}

  virtual void draw() const = 0;
  virtual void drawIndirect() const = 0;
};
}  // namespace gpu
