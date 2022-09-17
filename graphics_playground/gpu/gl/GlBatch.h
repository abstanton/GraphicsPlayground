#pragma once
#include "../GPUBatch.h"
#include <glad/glad.h>

namespace gpu {
	class GlBatch : public Batch
	{
	public:
		GlBatch(VertexBuffer* vert_buff);
		GlBatch(VertexBuffer* vert_buff, IndexBuffer* index_buff);
		~GlBatch();

		void draw() override;
		void drawIndirect() override;
	private:
		void vboBind();

		GLuint vao_id_;
	};
}
