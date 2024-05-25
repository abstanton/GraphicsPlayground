#include "GlShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "GlTexture.h"

namespace gp::gpu {
std::optional<GlShaderProgram> GlShaderProgram::fromFilepaths(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_LOADED\n" << std::endl;
    return {};
  }

  GlShaderProgram shader_program_;
  shader_program_.addShaderFromSource(ShaderType::VERT, vertexCode.c_str());
  shader_program_.addShaderFromSource(ShaderType::FRAG, fragmentCode.c_str());
  shader_program_.link();

  return shader_program_;
}

GlShaderProgram::GlShaderProgram() { shader_program_ = glCreateProgram(); }

GLuint glShaderTypeFromShaderType(ShaderType type) {
  switch (type) {
    case ShaderType::FRAG:
      return GL_FRAGMENT_SHADER;
    case ShaderType::VERT:
      return GL_VERTEX_SHADER;
    case ShaderType::COMPUTE:
      return GL_COMPUTE_SHADER;
    case ShaderType::GEOM:
      return GL_GEOMETRY_SHADER;
    default:
      return GL_FRAGMENT_SHADER;
  }
}

void GlShaderProgram::link() {
  for (const auto& [type, shader] : shaders_) {
    glAttachShader(shader_program_, shader.id);
  }
  GLint success;
  char info_log[512];
  glGetShaderiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
    return;
  }
  glLinkProgram(shader_program_);
}

void GlShaderProgram::addShaderFromSource(ShaderType type, const char* source) {
  GLuint gl_shader_type = glShaderTypeFromShaderType(type);
  GLuint shader = glCreateShader(gl_shader_type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint success;
  char info_log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return;
  }

  GlShader shader_info{shader};
  shaders_.emplace(gl_shader_type, shader_info);
}

void GlShaderProgram::use() const { glUseProgram(shader_program_); }

void GlShaderProgram::dispatch(int num_groups_x, int num_groups_y,
                               int num_groups_z) const {
  this->use();
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void GlShaderProgram::setTexture(const char* name,
                                 const gpu::Texture* texture) const {
  const GlTexture* gl_texture = static_cast<GlTexture const*>(texture);
  GLint loc = glGetUniformLocation(shader_program_, name);
  glUniformHandleui64ARB(loc, gl_texture->handle_);
}

void GlShaderProgram::setBool(const std::string& name, const bool value) const {
  glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), (int)value);
}
void GlShaderProgram::setInt(const std::string& name, const int value) const {
  glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), value);
}
void GlShaderProgram::setFloat(const std::string& name,
                               const float value) const {
  glUniform1f(glGetUniformLocation(shader_program_, name.c_str()), value);
}
void GlShaderProgram::setFloatArr(const std::string& name, const float* data,
                                  int count) const {
  glUniform1fv(glGetUniformLocation(shader_program_, name.c_str()), count,
               (GLfloat*)data);
}
void GlShaderProgram::setVec2(const std::string& name,
                              const glm::vec2& value) const {
  glUniform2f(glGetUniformLocation(shader_program_, name.c_str()), value.x,
              value.y);
}
void GlShaderProgram::setVec3(const std::string& name,
                              const glm::vec3& value) const {
  glUniform3f(glGetUniformLocation(shader_program_, name.c_str()), value.x,
              value.y, value.z);
}
void GlShaderProgram::setVec3Arr(const std::string& name, const glm::vec3* data,
                                 int count) const {
  glUniform3fv(glGetUniformLocation(shader_program_, name.c_str()), count,
               (GLfloat*)data);
}
void GlShaderProgram::setMat3(const std::string& name,
                              const glm::mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(shader_program_, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
void GlShaderProgram::setMat4(const std::string& name,
                              const glm::mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(shader_program_, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
}  // namespace gp::gpu