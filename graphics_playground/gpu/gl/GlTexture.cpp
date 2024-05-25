#include "GlTexture.h"

#include <iostream>

#include "GlTypes.h"
namespace gp::gpu {
GlTexture::GlTexture(TextureType type, TextureFormat format, DataType data_type,
                     unsigned int width, unsigned int height,
                     unsigned int depth, unsigned int mips, unsigned int levels,
                     const void* data, bool create_handle,
                     TextureFilter min_filter, TextureFilter mag_filter,
                     TextureWrapping wrap_s, TextureWrapping wrap_t,
                     glm::vec4 border_color)
    : Texture(type, format, data_type, width, height, depth, mips, levels) {
  glGenTextures(1, &id_);

  GLenum gl_format = textureFormatToGlFormat(format);
  GLenum gl_internal_format = textureFormatToGlInternalFormat(format);
  GLenum gl_data_type = dataTypeToGlDataType(data_type);
  GLenum gl_filter_min = textureFilterToGlFilter(min_filter);
  GLenum gl_filter_mag = textureFilterToGlFilter(mag_filter);
  GLenum gl_wrap_s = textureWrappingToGlWrapping(wrap_s);
  GLenum gl_wrap_t = textureWrappingToGlWrapping(wrap_t);

  // TODO: Use border_colour parameter
  float border_col[] = {0, 0, 0, 0};
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

  switch (type) {
    case TextureType::TEXTURE_1D:
    case TextureType::TEXTURE_1D_ARRAY:
      glBindTexture(GL_TEXTURE_1D, id_);
      glTexImage1D(GL_TEXTURE_1D, 0, gl_format, width, 0, gl_format,
                   gl_data_type, data);
      glBindTexture(GL_TEXTURE_1D, 0);
      break;
    case TextureType::TEXTURE_2D:
      glBindTexture(GL_TEXTURE_2D, id_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_mag);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap_s);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap_t);
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_col);
      glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0,
                   gl_format, gl_data_type, data);
      glBindTexture(GL_TEXTURE_2D, 0);
      break;
    case TextureType::TEXTURE_2D_ARRAY:
      create_handle = false;
      // TODO: currently don't support uploading data for texture array
      glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
      glTexStorage3D(GL_TEXTURE_2D_ARRAY, mips, gl_internal_format, width,
                     height, levels);

      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                      gl_filter_min);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER,
                      gl_filter_mag);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, gl_wrap_s);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, gl_wrap_t);
      glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR,
                       borderColor);
      glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
      break;
    case TextureType::TEXTURE_3D:
    case TextureType::TEXTURE_3D_ARRAY:
      glBindTexture(GL_TEXTURE_3D, id_);
      glTexImage3D(GL_TEXTURE_3D, 0, gl_format, width, height, depth, 0,
                   gl_format, gl_data_type, data);
      glBindTexture(GL_TEXTURE_3D, 0);
      break;
  }
  if (create_handle) {
    handle_ = glGetTextureHandleARB(id_);
    glMakeTextureHandleResidentARB(handle_);
  }
}

void GlTexture::bind(int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  switch (type) {
    case TextureType::TEXTURE_1D:
    case TextureType::TEXTURE_1D_ARRAY:
      glBindTexture(GL_TEXTURE_1D, id_);
      break;
    case TextureType::TEXTURE_2D:
      glBindTexture(GL_TEXTURE_2D, id_);
      break;
    case TextureType::TEXTURE_2D_ARRAY:
      glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
      break;
    case TextureType::TEXTURE_3D:
    case TextureType::TEXTURE_3D_ARRAY:
      glBindTexture(GL_TEXTURE_2D, id_);
      break;
  }
}

void GlTexture::generateMipmap() {
  bind(0);
  glGenerateMipmap(textureFormatToGlInternalFormat(format));
}

GlTexture::~GlTexture() { glDeleteTextures(1, &id_); }
}  // namespace gp::gpu