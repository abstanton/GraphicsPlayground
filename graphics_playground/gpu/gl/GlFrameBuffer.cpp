#include "GlFrameBuffer.h"

#include <iostream>
namespace gp::gpu {
GlFrameBuffer::GlFrameBuffer() { glGenFramebuffers(1, &id_); }

void GlFrameBuffer::bindAttachment(FrameBufferAttachmentType attachment_type,
                                   FrameBufferAttachment attachment) {
  GlTexture* gl_texture = static_cast<GlTexture*>(attachment.texture);
  GLenum gl_attachment_type = attachmentTypeToGlType(attachment_type);
  switch (attachment.texture->type) {
    case TextureType::TEXTURE_1D:
    case TextureType::TEXTURE_1D_ARRAY:
      glFramebufferTexture1D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_1D,
                             gl_texture->id_, attachment.mip);
      break;
    case TextureType::TEXTURE_2D:
      glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_2D,
                             gl_texture->id_, attachment.mip);
      break;
    case TextureType::TEXTURE_2D_ARRAY:
      glFramebufferTextureLayer(GL_FRAMEBUFFER, gl_attachment_type,
                                gl_texture->id_, attachment.mip,
                                attachment.layer);
      break;
    case TextureType::TEXTURE_3D:
    case TextureType::TEXTURE_3D_ARRAY:
      glFramebufferTexture3D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_3D,
                             gl_texture->id_, attachment.mip, attachment.layer);
      break;
  }
  switch (gl_attachment_type) {
    case GL_DEPTH_ATTACHMENT:
      break;
    default:
      break;
  }
}

void GlFrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, id_);
  bindAttachments();
  std::vector<GLenum> attachment_types;
  for (const auto& [attachment_type, attachment] : attachments_) {
    if (attachment_type == FrameBufferAttachmentType::DepthAttachment) continue;
    attachment_types.push_back(attachmentTypeToGlType(attachment_type));
  }
  if (attachment_types.size() < 1) return;
  glDrawBuffers(static_cast<int>(attachment_types.size()),
                attachment_types.data());
}

void GlFrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

GLenum GlFrameBuffer::attachmentTypeToGlType(
    FrameBufferAttachmentType type) const {
  switch (type) {
    case FrameBufferAttachmentType::ColorAttachment0:
      return GL_COLOR_ATTACHMENT0;
    case FrameBufferAttachmentType::ColorAttachment1:
      return GL_COLOR_ATTACHMENT1;
    case FrameBufferAttachmentType::ColorAttachment2:
      return GL_COLOR_ATTACHMENT2;
    case FrameBufferAttachmentType::ColorAttachment3:
      return GL_COLOR_ATTACHMENT3;
    case FrameBufferAttachmentType::ColorAttachment4:
      return GL_COLOR_ATTACHMENT4;
    case FrameBufferAttachmentType::ColorAttachment5:
      return GL_COLOR_ATTACHMENT5;
    case FrameBufferAttachmentType::DepthAttachment:
      return GL_DEPTH_ATTACHMENT;
    case FrameBufferAttachmentType::StencilAttachment:
      return GL_STENCIL_ATTACHMENT;
    default:
      return -1;
  }
}

GlFrameBuffer::~GlFrameBuffer() { glDeleteFramebuffers(1, &id_); }
}  // namespace gp::gpu
