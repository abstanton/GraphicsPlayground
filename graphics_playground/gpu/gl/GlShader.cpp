#include "GlShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "GlTexture.h"
namespace gpu {
std::optional<GlShader> GlShader::fromFilepaths(
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
  return GlShader(vertexCode.c_str(), fragmentCode.c_str());
}

GlShader::GlShader(const char* vertexSource, const char* fragmentSource) {
  int success;
  char infoLog[512];

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return;
  }

  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return;
  }

  shaderProgram_ = glCreateProgram();
  glAttachShader(shaderProgram_, vertexShader);
  glAttachShader(shaderProgram_, fragmentShader);
  glLinkProgram(shaderProgram_);

  glGetShaderiv(shaderProgram_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    return;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void GlShader::use() const { glUseProgram(shaderProgram_); }

void GlShader::setTexture(const char* name, const gpu::Texture* texture) const {
  const GlTexture* gl_texture = static_cast<GlTexture const*>(texture);
  GLint loc = glGetUniformLocation(shaderProgram_, name);
  glUniformHandleui64ARB(loc, gl_texture->handle_);
}

void GlShader::setBool(const std::string& name, const bool value) const {
  glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), (int)value);
}
void GlShader::setInt(const std::string& name, const int value) const {
  glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), value);
}
void GlShader::setFloat(const std::string& name, const float value) const {
  glUniform1f(glGetUniformLocation(shaderProgram_, name.c_str()), value);
}
void GlShader::setVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2f(glGetUniformLocation(shaderProgram_, name.c_str()), value.x,
              value.y);
}
void GlShader::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3f(glGetUniformLocation(shaderProgram_, name.c_str()), value.x,
              value.y, value.z);
}
void GlShader::setVec3Arr(const std::string& name, const glm::vec3* data,
                          int count) const {
  glUniform3fv(glGetUniformLocation(shaderProgram_, name.c_str()), count,
               (GLfloat*)data);
}
void GlShader::setMat3(const std::string& name, const glm::mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
void GlShader::setMat4(const std::string& name, const glm::mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
}  // namespace gpu