#pragma once
#include <glad/glad.h>

#include "GPUTypes.h"

namespace gpu {
class Texture {
 public:
  Texture(TextureType type, TextureFormat format, DataType data_type,
          unsigned int width, unsigned int height, unsigned int depth,
          unsigned int mips, unsigned int levels, bool create_handle = true)
      : type(type),
        format(format),
        data_type(data_type),
        width(width),
        height(height),
        depth(depth),
        mips(mips),
        levels(levels) {}

  virtual void bind(int slot) = 0;
  virtual void generateMipmap() = 0;

  TextureFormat format;
  DataType data_type;
  TextureType type;

  unsigned int width;
  unsigned int height;
  unsigned int depth;
  unsigned int mips;
  unsigned int levels;
};
}  // namespace gpu
