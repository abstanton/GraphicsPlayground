#pragma once
#include "GPUTexture.h"

namespace gpu {
enum class TextureAttachmentType {
  DepthAttachment,
  StencilAttachment,
  ColorAttachement
};

class FrameBuffer {
 public:
  virtual void attachTexture(Texture* texture,
                             TextureAttachmentType attachment_type, int level,
                             int mip) = 0;
  virtual void bind() = 0;
  virtual void unbind() = 0;
};
}  // namespace gpu
