#pragma once
#include <string>

#include "IResource.h"

enum class TextureFormat {
  R,

  R8,
  R16,
  R32,

  RGB8,
  RGB16,
  RGB32,
  RGBA8,
  RGBA16,
  RGBA32,

  RGB8F,
  RGB16F,
  RGB32F,
  RGBA8F,
  RGBA16F,
  RGBA32F,

  SRGB8,
  SRGB16,
  SRGB32,
  SRGBA8,
  SRGBA16,
  SRGBA32,

  SRGB8F,
  SRGB16F,
  SRGB32F,
  SRGBA8F,
  SRGBA16F,
  SRGBA32F,
};

struct Texture : public IResource {
 public:
  Texture() : width(0), height(0) {}
  Texture(const unsigned char* data, const int width, const int height,
          TextureFormat format);

  const int width;
  const int height;
  const unsigned char* data;
  TextureFormat format;
};
