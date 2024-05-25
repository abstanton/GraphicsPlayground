#pragma once
#include <memory>

namespace gp::gpu {
class IndexBuffer {
 public:
  size_t count() { return num_indices_; }

  // TODO: Support different data types for indices
  virtual void uploadData(size_t num_indices, size_t data_size,
                          const void* data) = 0;
  virtual void bind() = 0;

 protected:
  size_t num_indices_;
};
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
}  // namespace gp::gpu
