#pragma once
#include "../GPUUniformBuffer.h"
#include <glad/glad.h>

namespace gpu {
	class GlUniformBuffer : public UniformBuffer
	{
	public:
		GlUniformBuffer(size_t size);
		~GlUniformBuffer();
		void uploadData(void* data) override;
		void bind() override;
		void bind(int slot) override;
	private:
		GLuint id_;
	};
}

