#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "GPUTexture.h"
namespace gp::gpu {
enum class FrameBufferAttachmentType {
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
  int layer;
  int mip;
};

class FrameBuffer {
 public:
  virtual void addAttachment(FrameBufferAttachmentType attachment_type,
                             FrameBufferAttachment attachment) {
    attachments_[attachment_type] = attachment;
  }
  virtual void clearAttachments() { attachments_.clear(); }
  virtual void bindAttachments() {
    for (const auto& [attachment_type, attachment] : attachments_) {
      bindAttachment(attachment_type, attachment);
    }
  }
  virtual void bind() = 0;
  virtual void unbind() = 0;

 protected:
  virtual void bindAttachment(FrameBufferAttachmentType attachment_type,
                              FrameBufferAttachment attachment) = 0;
  std::unordered_map<FrameBufferAttachmentType, FrameBufferAttachment>
      attachments_;
};

using FrameBufferPtr = std::shared_ptr<FrameBuffer>;

}  // namespace gp::gpu
