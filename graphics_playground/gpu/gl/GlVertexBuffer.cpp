#include "GlVertexBuffer.h"
#include "GlTypes.h"

namespace gpu {
	GlVertexBuffer::GlVertexBuffer() {
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GlVertexBuffer::~GlVertexBuffer() {
		glDeleteBuffers(1, &id_);
	}

	void GlVertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, id_);
	}

	void GlVertexBuffer::uploadData(std::vector<VertexAttrib> vertex_attribs, size_t num_verts, size_t data_size, const void* data){
		
		bind();
		//TODO: Abstracterise usage
		glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
		vertex_attribs_ = vertex_attribs;
		num_verts_ = num_verts;
	}
}