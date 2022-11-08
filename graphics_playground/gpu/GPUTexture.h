#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "GPUTypes.h"

namespace gpu {

class Texture {
 public:
  Texture(TextureType type, TextureFormat format, DataType data_type,
          unsigned int width, unsigned int height, unsigned int depth,
          unsigned int mips, unsigned int levels, bool create_handle = true,
          TextureFilter min_filter = TextureFilter::NEAREST,
          TextureFilter mag_filter = TextureFilter::NEAREST,
          TextureWrapping wrap_s = TextureWrapping::REPEAT,
          TextureWrapping wrap_t = TextureWrapping::REPEAT,
          glm::vec4 border_color = {0, 0, 0, 0})
      : type(type),
        format(format),
        data_type(data_type),
        width(width),
        height(height),
        depth(depth),
        mips(mips),
        levels(levels) {}

  virtual void bind(int slot) const = 0;
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
