#include "Texture.h"

#include <glad/glad.h>

Texture::Texture(const unsigned char* data, const int width, const int height,
                 TextureFormat format)
    : data(data), width(width), height(height), format(format) {}