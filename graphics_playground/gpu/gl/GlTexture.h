#pragma once
#include "../GPUTexture.h"
#include <glad/glad.h>

namespace gpu {
	class GlTexture : public Texture
	{
	public:
		GlTexture();
		~GlTexture();

		GlTexture(
			TextureType type,
			TextureFormat format,
			DataType data_type,
			unsigned int width,
			unsigned int height,
			unsigned int depth,
			unsigned int mips,
			unsigned int levels,
			const void* data);

		void bind(int slot) override;
		void generateMipmap() override;

		GLuint id_;
	};
}

