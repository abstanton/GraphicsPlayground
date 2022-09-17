#pragma once
#include "GPUTypes.h"
#include <vector>

namespace gpu {
	struct VertexAttrib {
		const char* label;
		int index;
		int num;
		DataType type;
		bool normalized; //TODO: What does this do?
		int stride;
		void* offset;
	};

	class VertexBuffer
	{
	public:
		size_t count() { return num_verts_; }
		std::vector<VertexAttrib> vertex_attribs_;


		virtual void bind() = 0;
		virtual void uploadData(std::vector<VertexAttrib> vertex_attribs, size_t num_verts, size_t data_size, const void* data) = 0;
	protected:
		size_t num_verts_ = 0;
	};
}