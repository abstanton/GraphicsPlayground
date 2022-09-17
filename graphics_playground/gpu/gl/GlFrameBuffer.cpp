#include "GlFrameBuffer.h"
#include <iostream>
namespace gpu {
	GlFrameBuffer::GlFrameBuffer() {
		glGenFramebuffers(1, &id_);
	}

	void GlFrameBuffer::attachTexture(Texture* texture, TextureAttachmentType attachment_type, int level, int mip) {
		GlTexture* gl_texture = static_cast<GlTexture*>(texture);
		GLenum gl_attachment_type = attachmentTypeToGlType(attachment_type);
		this->bind();
		switch (texture->type) {
		case TextureType::TEXTURE_1D:
		case TextureType::TEXTURE_1D_ARRAY:
			glFramebufferTexture1D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_1D, gl_texture->id_, 0);
			break;
		case TextureType::TEXTURE_2D:
		case TextureType::TEXTURE_2D_ARRAY:
			glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_2D, gl_texture->id_, 0);
			break;
		case TextureType::TEXTURE_3D:
		case TextureType::TEXTURE_3D_ARRAY:
			glFramebufferTexture3D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_3D, gl_texture->id_, 0, 0);
			break;
		}
		switch (gl_attachment_type) {
		case GL_DEPTH_ATTACHMENT:
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			break;
		default:
			break;
		}
		this->unbind();
	}

	void GlFrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, id_);
	}

	void GlFrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLenum GlFrameBuffer::attachmentTypeToGlType(TextureAttachmentType type) const {
		switch (type) {
		case TextureAttachmentType::ColorAttachement:
			return GL_COLOR_ATTACHMENT0;
		case TextureAttachmentType::DepthAttachment:
			return GL_DEPTH_ATTACHMENT;
		case TextureAttachmentType::StencilAttachment:
			return GL_STENCIL_ATTACHMENT;
		default:
			return -1;
		}
	}

	GlFrameBuffer::~GlFrameBuffer() {
		glDeleteFramebuffers(1, &id_);
	}
}
