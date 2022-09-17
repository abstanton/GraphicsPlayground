#include "GlTexture.h"
#include "GlTypes.h"

#include <iostream>
namespace gpu {
	GlTexture::GlTexture(
		TextureType type,
		TextureFormat format,
		DataType data_type,
		unsigned int width,
		unsigned int height,
		unsigned int depth,
		unsigned int mips,
		unsigned int levels,
		const void* data) :
		Texture(
			type,
			format,
			data_type,
			width,
			height,
			depth,
			mips,
			levels) {
		glGenTextures(1, &id_);

		GLenum gl_format = textureFormatToGlFormat(format);
		GLenum gl_data_type = dataTypeToGlDataType(data_type);

		// TODO: Support texture arrays
		switch (type) {
		case TextureType::TEXTURE_1D:
		case TextureType::TEXTURE_1D_ARRAY:
			glBindTexture(GL_TEXTURE_1D, id_);
			glTexImage1D(GL_TEXTURE_1D, 0, gl_format, width, 0, gl_format, gl_data_type, data);
			glBindTexture(GL_TEXTURE_1D, 0);
			break;
		case TextureType::TEXTURE_2D:
		case TextureType::TEXTURE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D, id_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, gl_data_type, data);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		case TextureType::TEXTURE_3D:
		case TextureType::TEXTURE_3D_ARRAY:
			glBindTexture(GL_TEXTURE_3D, id_);
			glTexImage3D(GL_TEXTURE_3D, 0, gl_format, width, height, depth, 0, gl_format, gl_data_type, data);
			glBindTexture(GL_TEXTURE_3D, 0);
			break;
		}
	}

	void GlTexture::bind(int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		switch (type) {
		case TextureType::TEXTURE_1D:
		case TextureType::TEXTURE_1D_ARRAY:
			glBindTexture(GL_TEXTURE_1D, id_);
			break;
		case TextureType::TEXTURE_2D:
		case TextureType::TEXTURE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D, id_);
			break;
		case TextureType::TEXTURE_3D:
		case TextureType::TEXTURE_3D_ARRAY:
			glBindTexture(GL_TEXTURE_2D, id_);
			break;
		}
	}

	void GlTexture::generateMipmap() {
		bind(0);
		glGenerateMipmap(textureFormatToGlFormat(format));
	}

	GlTexture::~GlTexture() {
		glDeleteTextures(1, &id_);
	}
}