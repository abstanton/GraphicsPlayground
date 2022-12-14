#pragma once
#include <glad/glad.h>

#include "../GPUTypes.h"

namespace gpu {
inline GLenum textureFormatToGlInternalFormat(TextureFormat format) {
  switch (format) {
    case TextureFormat::DEPTH:
      return GL_DEPTH_COMPONENT;
    case TextureFormat::DEPTH_24:
      return GL_DEPTH_COMPONENT24;
    case TextureFormat::DEPTH_32:
      return GL_DEPTH_COMPONENT32F;
    case TextureFormat::R:
      return GL_RED;
    case TextureFormat::RGB:
      return GL_RGB;
    case TextureFormat::RGBA:
      return GL_RGBA;
    case TextureFormat::RG:
      return GL_RG;
    case TextureFormat::RG_16F:
      return GL_RG16F;
    case TextureFormat::RG_32F:
      return GL_RG32F;
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

inline GLenum textureFormatToGlFormat(TextureFormat format) {
  switch (format) {
    case TextureFormat::DEPTH:
    case TextureFormat::DEPTH_24:
    case TextureFormat::DEPTH_32:
      return GL_DEPTH_COMPONENT;
    case TextureFormat::R_16F:
    case TextureFormat::R_32F:
    case TextureFormat::R:
      return GL_RED;
    case TextureFormat::RG:
    case TextureFormat::RG_16F:
    case TextureFormat::RG_32F:
      return GL_RG;
    case TextureFormat::RGB:
    case TextureFormat::RGB_16F:
    case TextureFormat::RGB_32F:
      return GL_RGB;
    case TextureFormat::RGBA:
    case TextureFormat::RGBA_16F:
    case TextureFormat::RGBA_32F:
      return GL_RGBA;
    default:
      return -1;
  }
}

inline GLenum textureFilterToGlFilter(TextureFilter filter) {
  switch (filter) {
    case TextureFilter::LINEAR:
      return GL_LINEAR;
    case TextureFilter::NEAREST:
      return GL_NEAREST;
    default:
      return GL_NEAREST;
  }
}

inline GLenum textureWrappingToGlWrapping(TextureWrapping wrapping) {
  switch (wrapping) {
    case TextureWrapping::CLAMP_TO_BORDER:
      return GL_CLAMP_TO_BORDER;
    case TextureWrapping::CLAMP_TO_EDGE:
      return GL_CLAMP_TO_EDGE;
    case TextureWrapping::REPEAT:
      return GL_REPEAT;
    case TextureWrapping::MIRRORED_REPEAT:
      return GL_MIRRORED_REPEAT;
    default:
      return GL_REPEAT;
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
}  // namespace gpu
