#pragma once
#include <glad/glad.h>

#include "../GPUFrameBuffer.h"
#include "GlTexture.h"

namespace gpu {
class GlFrameBuffer : public FrameBuffer {
 public:
  GlFrameBuffer();
  void attachTexture(Texture* texture, TextureAttachmentType attachment_type,
                     int level, int mip) override;
  void bind() override;
  void unbind() override;
  ~GlFrameBuffer();

 private:
  GLuint id_;
  GLenum attachmentTypeToGlType(TextureAttachmentType type) const;
};
}  // namespace gpu
