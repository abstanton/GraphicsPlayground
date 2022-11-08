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
            bool create_handle = true,
            TextureFilter min_filter = TextureFilter::NEAREST,
            TextureFilter mag_filter = TextureFilter::NEAREST,
            TextureWrapping wrap_s = TextureWrapping::REPEAT,
            TextureWrapping wrap_t = TextureWrapping::REPEAT,
            glm::vec4 border_color = {0, 0, 0, 0});

  void bind(int slot) const override;
  void generateMipmap() override;

  GLuint id_;
  GLuint64 handle_;
};
}  // namespace gpu
