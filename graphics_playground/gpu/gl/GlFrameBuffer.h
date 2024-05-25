#pragma once
#include <glad/glad.h>

#include <vector>

#include "../GPUFrameBuffer.h"
#include "GlTexture.h"

namespace gp::gpu {
class GlFrameBuffer : public FrameBuffer {
 public:
  GlFrameBuffer();
  GlFrameBuffer(GLuint id) : id_(id) {}

  static FrameBufferPtr defaultFrameBuffer() {
    return std::make_shared<GlFrameBuffer>(0);
  }

  void bindAttachment(FrameBufferAttachmentType attachment_type,
                      FrameBufferAttachment attachment) override;
  void bind() override;
  void unbind() override;
  ~GlFrameBuffer();

 private:
  GLuint id_;
  GLenum attachmentTypeToGlType(FrameBufferAttachmentType type) const;
};
}  // namespace gp::gpu
