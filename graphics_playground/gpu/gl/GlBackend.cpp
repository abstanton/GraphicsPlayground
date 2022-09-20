#include "GlBackend.h"
namespace gpu {
static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       const void* userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}

GlBackend::GlBackend() {
  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
}

FrameBuffer* GlBackend::defaultFrameBuffer() { return new GlFrameBuffer(0); }
FrameBuffer* GlBackend::allocFrameBuffer() { return new GlFrameBuffer(); }

UniformBuffer* GlBackend::allocUniformBuffer(size_t size) {
  return new GlUniformBuffer(size);
}

VertexBuffer* GlBackend::allocVertexBuffer() { return new GlVertexBuffer(); }

IndexBuffer* GlBackend::allocIndexBuffer() { return new GlIndexBuffer(); }

Batch* GlBackend::allocBatch(VertexBuffer* vert_buff) {
  return new GlBatch(vert_buff);
}
Batch* GlBackend::allocBatch(VertexBuffer* vert_buff, IndexBuffer* index_buff) {
  return new GlBatch(vert_buff, index_buff);
}

Shader* GlBackend::compileShaderProgram(const char* vs_source,
                                        const char* fs_source) {
  return new GlShader(vs_source, fs_source);
}

Texture* GlBackend::generateTexture(TextureType type, TextureFormat format,
                                    DataType data_type, unsigned int width,
                                    unsigned int height, unsigned int depth,
                                    unsigned int mips, unsigned int levels,
                                    const void* data) {
  return new GlTexture(type, format, data_type, width, height, depth, mips,
                       levels, data);
}

void GlBackend::setViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void GlBackend::unbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace gpu