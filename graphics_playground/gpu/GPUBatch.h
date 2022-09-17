#pragma once
#include "GPUVertexBuffer.h"
#include "GPUIndexBuffer.h"

namespace gpu {
	class Batch
	{
	protected:
		VertexBuffer* vert_buff_ = nullptr;
		IndexBuffer* index_buff_ = nullptr;
	public:
		Batch(VertexBuffer* vert_buff) : vert_buff_(vert_buff) {}
		Batch(VertexBuffer* vert_buff, IndexBuffer* index_buff) : vert_buff_(vert_buff), index_buff_(index_buff) {}

		virtual void draw() = 0;
		virtual void drawIndirect() = 0;
	};
}

