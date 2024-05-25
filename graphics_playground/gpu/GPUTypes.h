#pragma once
namespace gp::gpu {
enum class TextureType {
  TEXTURE_1D,
  TEXTURE_1D_ARRAY,
  TEXTURE_2D,
  TEXTURE_2D_ARRAY,
  TEXTURE_3D,
  TEXTURE_3D_ARRAY,
};

enum class TextureFormat {
  R,
  RG,
  RGB,
  RGBA,
  RG_16F,
  RG_32F,
  RGB_16F,
  RGB_32F,
  RGBA_16F,
  RGBA_32F,
  R_16F,
  R_32F,
  DEPTH,
  DEPTH_24,
  DEPTH_32
};

enum class TextureFilter { LINEAR, NEAREST };

enum class TextureWrapping {
  CLAMP_TO_BORDER,
  CLAMP_TO_EDGE,
  REPEAT,
  MIRRORED_REPEAT
};

enum class DataType {
  FLOAT,
  UNSIGNED_BYTE,
};
}  // namespace gp::gpu