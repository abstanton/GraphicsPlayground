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

FrameBufferPtr GlBackend::defaultFrameBuffer() {
  return GlFrameBuffer::defaultFrameBuffer();
}

FrameBufferPtr GlBackend::allocFrameBuffer() {
  return std::make_shared<GlFrameBuffer>();
}

UniformBufferPtr GlBackend::allocUniformBuffer(size_t size) {
  return std::make_shared<GlUniformBuffer>(size);
}

VertexBufferPtr GlBackend::allocVertexBuffer() {
  return std::make_shared<GlVertexBuffer>();
}

IndexBufferPtr GlBackend::allocIndexBuffer() {
  return std::make_shared<GlIndexBuffer>();
}

BatchPtr GlBackend::allocBatch(VertexBufferPtr vert_buff) {
  return std::make_shared<GlBatch>(vert_buff);
}
BatchPtr GlBackend::allocBatch(VertexBufferPtr vert_buff,
                               IndexBufferPtr index_buff) {
  return std::make_shared<GlBatch>(vert_buff, index_buff);
}

ShaderProgramPtr GlBackend::allocShaderProgram() {
  auto shader_program = std::make_shared<GlShaderProgram>();
  return shader_program;
}

TexturePtr GlBackend::generateTexture(
    TextureType type, TextureFormat format, DataType data_type,
    unsigned int width, unsigned int height, unsigned int depth,
    unsigned int mips, unsigned int levels, const void* data,
    TextureFilter min_filter, TextureFilter mag_filter, TextureWrapping wrap_s,
    TextureWrapping wrap_t) {
  return std::make_shared<GlTexture>(type, format, data_type, width, height,
                                     depth, mips, levels, data, true,
                                     min_filter, mag_filter, wrap_s, wrap_t);
}

void GlBackend::setViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void GlBackend::unbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace gpu