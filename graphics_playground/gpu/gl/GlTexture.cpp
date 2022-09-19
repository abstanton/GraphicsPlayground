#include "GlTexture.h"

#include <iostream>

#include "GlTypes.h"
namespace gpu {
GlTexture::GlTexture(TextureType type, TextureFormat format, DataType data_type,
                     unsigned int width, unsigned int height,
                     unsigned int depth, unsigned int mips, unsigned int levels,
                     const void* data)
    : Texture(type, format, data_type, width, height, depth, mips, levels) {
  glGenTextures(1, &id_);

  GLenum gl_format = textureFormatToGlFormat(format);
  GLenum gl_data_type = dataTypeToGlDataType(data_type);

  float border_col[] = {0, 0, 0, 0};
  bool create_handle = true;
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
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_col);
      glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format,
                   gl_data_type, data);
      glBindTexture(GL_TEXTURE_2D, 0);
      break;
    case TextureType::TEXTURE_2D_ARRAY:
      create_handle = false;
      // TODO: currently don't support uploading data for texture array
      glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
      // TODO: this is a messy way to use DEPTH_COMPONENT32F rather than raw
      // DEPTH_COMPONENT
      gl_format =
          gl_format == GL_DEPTH_COMPONENT ? GL_DEPTH_COMPONENT32F : gl_format;
      glTexStorage3D(GL_TEXTURE_2D_ARRAY, mips, gl_format, width, height,
                     levels);

      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_BORDER);
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

void GlTexture::bind(int slot) {
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
  glGenerateMipmap(textureFormatToGlFormat(format));
}

GlTexture::~GlTexture() { glDeleteTextures(1, &id_); }
}  // namespace gpu