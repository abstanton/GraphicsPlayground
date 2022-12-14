#pragma once
#include <memory>

namespace gpu {

class UniformBuffer {
 public:
  UniformBuffer(size_t size) : size(size) {}
  virtual void uploadData(void* data) = 0;
  virtual void bind() = 0;
  virtual void bind(int slot) = 0;

 protected:
  size_t size;
};
using UniformBufferPtr = std::shared_ptr<UniformBuffer>;

}  // namespace gpu
