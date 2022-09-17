#pragma once
#include "../GPUTypes.h"
#include <glad/glad.h>

namespace gpu {
	inline GLenum textureFormatToGlFormat(TextureFormat format) {
		switch (format) {
		case TextureFormat::DEPTH:
			return GL_DEPTH_COMPONENT;
		case TextureFormat::R:
			return GL_RED;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		case TextureFormat::RGBA_16F:
			return GL_RGBA16F;
		case TextureFormat::RGBA_32F:
			return GL_RGBA32F;
		case TextureFormat::RGB_16F:
			return GL_RGB16F;
		case TextureFormat::RGB_32F:
			return GL_RGB32F;
		case TextureFormat::R_16F:
			return GL_R16F;
		case TextureFormat::R_32F:
			return GL_R32F;
		default:
			return -1;
		}
	}

	inline GLenum dataTypeToGlDataType(DataType data_type) {
		switch (data_type) {
		case DataType::FLOAT:
			return GL_FLOAT;
		case DataType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		default:
			return -1;
		}
	}
}
