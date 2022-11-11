#pragma once
#include <memory>
#include <vector>

#include "GPUTypes.h"

namespace gpu {
struct VertexAttrib {
  const char* label;
  int index;
  int num;
  DataType type;
  bool normalized;
  int stride;
  void* offset;
};

class VertexBuffer {
  using VertexBufferPtr = std::shared_ptr<VertexBuffer>;

 public:
  size_t count() { return num_verts_; }
  std::vector<VertexAttrib> vertex_attribs_;

  virtual void bind() = 0;
  virtual void uploadData(std::vector<VertexAttrib> vertex_attribs,
                          size_t num_verts, size_t data_size,
                          const void* data) = 0;

 protected:
  size_t num_verts_ = 0;
};
}  // namespace gpu