#include "GlIndexBuffer.h"
namespace gp::gpu {
GlIndexBuffer::GlIndexBuffer() {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GlIndexBuffer::~GlIndexBuffer() { glDeleteBuffers(1, &id_); }

void GlIndexBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }

void GlIndexBuffer::uploadData(size_t num_indices, size_t data_size,
                               const void* data) {
  bind();
  // TODO: ABSTRACTERISE USAGE
  num_indices_ = num_indices;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
}
}  // namespace gp::gpu