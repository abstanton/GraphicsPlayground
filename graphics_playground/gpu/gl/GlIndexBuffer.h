#pragma once
#include "../GPUIndexBuffer.h"
#include <glad/glad.h>

namespace gpu {
	class GlIndexBuffer : public IndexBuffer
	{
	public:
		GlIndexBuffer();
		~GlIndexBuffer();

		void bind() override;
		void uploadData(size_t num_indices, size_t data_size, const void* data) override;
	private:
		GLuint id_;
	};
}

