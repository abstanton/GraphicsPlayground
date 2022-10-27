#pragma once
#include <glad/glad.h>

#include <vector>

#include "../GPUFrameBuffer.h"
#include "GlTexture.h"

namespace gpu {
class GlFrameBuffer : public FrameBuffer {
 public:
  GlFrameBuffer();
  GlFrameBuffer(GLuint id) : id_(id) {}

  static GlFrameBuffer* defaultFrameBuffer() {
    GlFrameBuffer* fb = new GlFrameBuffer(0);
    return fb;
  }

  void bindAttachment(FrameBufferAttachment attachment) override;
  void bind() override;
  void unbind() override;
  ~GlFrameBuffer();

 private:
  GLuint id_;
  GLenum attachmentTypeToGlType(TextureAttachmentType type) const;
};
}  // namespace gpu
