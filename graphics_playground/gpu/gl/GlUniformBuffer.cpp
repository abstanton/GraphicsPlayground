#include "GlUniformBuffer.h"
namespace gp::gpu {
GlUniformBuffer::GlUniformBuffer(size_t size) : UniformBuffer(size) {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_UNIFORM_BUFFER, id_);
  glBufferData(GL_UNIFORM_BUFFER, size, (void*)0, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlUniformBuffer::uploadData(void* data) {
  this->bind();
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlUniformBuffer::bind() { glBindBuffer(GL_UNIFORM_BUFFER, id_); }

void GlUniformBuffer::bind(int slot) {
  glBindBuffer(GL_UNIFORM_BUFFER, id_);
  glBindBufferBase(GL_UNIFORM_BUFFER, slot, id_);
}

GlUniformBuffer::~GlUniformBuffer() { glDeleteBuffers(1, &id_); }
}  // namespace gp::gpu