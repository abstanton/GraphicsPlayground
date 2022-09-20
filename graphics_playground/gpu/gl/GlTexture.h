#pragma once
#include <glad/glad.h>

#include "../GPUTexture.h"

namespace gpu {
class GlTexture : public Texture {
 public:
  GlTexture();
  ~GlTexture();

  GlTexture(TextureType type, TextureFormat format, DataType data_type,
            unsigned int width, unsigned int height, unsigned int depth,
            unsigned int mips, unsigned int levels, const void* data,
            bool create_handle = true);

  void bind(int slot) override;
  void generateMipmap() override;

  GLuint id_;
  GLuint64 handle_;
};
}  // namespace gpu
