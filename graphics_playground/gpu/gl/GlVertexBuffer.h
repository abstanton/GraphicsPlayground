#pragma once
#include "../GPUVertexBuffer.h"
#include <glad/glad.h>
namespace gpu {
	class GlVertexBuffer : public VertexBuffer
	{
	public:
		GlVertexBuffer();
		~GlVertexBuffer();

		void bind() override;
		void uploadData(std::vector<VertexAttrib> vertex_attribs, size_t num_verts, size_t data_size, const void* data) override;
	private:
		GLuint id_;
	};
}

