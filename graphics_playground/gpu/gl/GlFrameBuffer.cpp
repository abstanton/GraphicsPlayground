#include "GlFrameBuffer.h"

#include <iostream>
namespace gpu {
GlFrameBuffer::GlFrameBuffer() { glGenFramebuffers(1, &id_); }

void GlFrameBuffer::attachTexture(Texture* texture,
                                  TextureAttachmentType attachment_type,
                                  int layer, int mip) {
  GlTexture* gl_texture = static_cast<GlTexture*>(texture);
  GLenum gl_attachment_type = attachmentTypeToGlType(attachment_type);
  if (attachment_type != TextureAttachmentType::DepthAttachment &&
      attachment_type != TextureAttachmentType::StencilAttachment &&
      attachment_type != TextureAttachmentType::ColorAttachment0) {
    color_attachments_.push_back(gl_attachment_type);
  }
  this->bind();
  switch (texture->type) {
    case TextureType::TEXTURE_1D:
    case TextureType::TEXTURE_1D_ARRAY:
      glFramebufferTexture1D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_1D,
                             gl_texture->id_, 0);
      break;
    case TextureType::TEXTURE_2D:
      glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_2D,
                             gl_texture->id_, 0);
      break;
    case TextureType::TEXTURE_2D_ARRAY:
      glFramebufferTextureLayer(GL_FRAMEBUFFER, gl_attachment_type,
                                gl_texture->id_, mip, layer);
      glGetError();
      break;
    case TextureType::TEXTURE_3D:
    case TextureType::TEXTURE_3D_ARRAY:
      glFramebufferTexture3D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_3D,
                             gl_texture->id_, 0, 0);
      break;
  }
  switch (gl_attachment_type) {
    case GL_DEPTH_ATTACHMENT:
      break;
    default:
      break;
  }
}

void GlFrameBuffer::clearAttachments() {
  color_attachments_ = {GL_COLOR_ATTACHMENT0};
}

void GlFrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, id_);
  if (color_attachments_.size() >= 1) {
    glDrawBuffers(color_attachments_.size(), color_attachments_.data());
  }
}

void GlFrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

GLenum GlFrameBuffer::attachmentTypeToGlType(TextureAttachmentType type) const {
  switch (type) {
    case TextureAttachmentType::ColorAttachment0:
      return GL_COLOR_ATTACHMENT0;
    case TextureAttachmentType::ColorAttachment1:
      return GL_COLOR_ATTACHMENT1;
    case TextureAttachmentType::ColorAttachment2:
      return GL_COLOR_ATTACHMENT2;
    case TextureAttachmentType::ColorAttachment3:
      return GL_COLOR_ATTACHMENT3;
    case TextureAttachmentType::ColorAttachment4:
      return GL_COLOR_ATTACHMENT4;
    case TextureAttachmentType::ColorAttachment5:
      return GL_COLOR_ATTACHMENT5;
    case TextureAttachmentType::DepthAttachment:
      return GL_DEPTH_ATTACHMENT;
    case TextureAttachmentType::StencilAttachment:
      return GL_STENCIL_ATTACHMENT;
    default:
      return -1;
  }
}

GlFrameBuffer::~GlFrameBuffer() { glDeleteFramebuffers(1, &id_); }
}  // namespace gpu
