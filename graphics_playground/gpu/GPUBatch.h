#pragma once
#include <memory>

#include "GPUIndexBuffer.h"
#include "GPUVertexBuffer.h"

namespace gp::gpu {

class Batch {
 protected:
  VertexBufferPtr vert_buff_ = nullptr;
  IndexBufferPtr index_buff_ = nullptr;

 public:
  Batch(VertexBufferPtr vert_buff) : vert_buff_(vert_buff) {}
  Batch(VertexBufferPtr vert_buff, IndexBufferPtr index_buff)
      : vert_buff_(vert_buff), index_buff_(index_buff) {}

  virtual void draw() const = 0;
  virtual void drawIndirect() const = 0;
};
using BatchPtr = std::shared_ptr<Batch>;

}  // namespace gp::gpu
