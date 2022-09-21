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
    fb->color_attachments_ = {};
    return fb;
  }

  void attachTexture(Texture* texture, TextureAttachmentType attachment_type,
                     int level, int mip) override;
  void clearAttachments() override;
  void bind() override;
  void unbind() override;
  ~GlFrameBuffer();

 private:
  std::vector<GLenum> color_attachments_ = {GL_COLOR_ATTACHMENT0};
  GLuint id_;
  GLenum attachmentTypeToGlType(TextureAttachmentType type) const;
};
}  // namespace gpu
