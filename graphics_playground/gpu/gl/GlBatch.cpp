#include "GlBatch.h"

#include "GlTypes.h"

namespace gp::gpu {
void GlBatch::vboBind() {
  vert_buff_->bind();
  for (const auto& v : vert_buff_->vertex_attribs_) {
    glVertexAttribPointer(v.index, v.num, dataTypeToGlDataType(v.type),
                          v.normalized, v.stride, v.offset);
    glEnableVertexAttribArray(v.index);
  }
}

GlBatch::GlBatch(VertexBufferPtr vert_buff) : Batch(vert_buff) {
  glGenVertexArrays(1, &vao_id_);
  glBindVertexArray(vao_id_);
  vboBind();
  glBindVertexArray(0);
}

GlBatch::GlBatch(VertexBufferPtr vert_buff, IndexBufferPtr index_buff)
    : Batch(vert_buff, index_buff) {
  glGenVertexArrays(1, &vao_id_);
  glBindVertexArray(vao_id_);
  vboBind();
  index_buff_->bind();
  glBindVertexArray(0);
}

void GlBatch::draw() const {
  glBindVertexArray(vao_id_);
  if (index_buff_ != nullptr) {
    glDrawElements(GL_TRIANGLES, index_buff_->count(), GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, vert_buff_->count());
  }
  glBindVertexArray(0);
}

void GlBatch::drawIndirect() const {}
}  // namespace gp::gpu