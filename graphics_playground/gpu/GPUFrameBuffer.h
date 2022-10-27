#pragma once
#include <vector>

#include "GPUTexture.h"

namespace gpu {
enum class TextureAttachmentType {
  DepthAttachment,
  StencilAttachment,
  ColorAttachment0,
  ColorAttachment1,
  ColorAttachment2,
  ColorAttachment3,
  ColorAttachment4,
  ColorAttachment5
};

struct FrameBufferAttachment {
  Texture* texture;
  TextureAttachmentType attachment_type;
  int layer;
  int mip;
};

class FrameBuffer {
 public:
  virtual void addAttachment(FrameBufferAttachment attachment) {
    attachments_.push_back(attachment);
  }
  virtual void clearAttachments() { attachments_.clear(); }
  virtual void bindAttachments() {
    for (auto& attachment : attachments_) {
      bindAttachment(attachment);
    }
  }
  virtual void bind() = 0;
  virtual void unbind() = 0;

 protected:
  virtual void bindAttachment(FrameBufferAttachment attachment) = 0;
  std::vector<FrameBufferAttachment> attachments_ = {};
};
}  // namespace gpu
