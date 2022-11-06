#pragma once

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../GPUShaderProgram.h"

namespace gpu {

struct GlShader {
  GLuint id;
};

class GlShaderProgram : public ShaderProgram {
 public:
  static std::optional<GlShaderProgram> fromFilepaths(
      const std::string& vertexShaderPath,
      const std::string& fragmentShaderPath);

  GlShaderProgram();
  void use() const override;
  void link() override;

  void addShaderFromSource(ShaderType, const char*) override;

  void dispatch(int num_groups_x, int num_groups_y,
                int num_groups_z) const override;

  void setTexture(const char* name, const gpu::Texture* texture) const override;
  void setBool(const std::string& name, const bool value) const override;
  void setInt(const std::string& name, const int value) const override;
  void setFloat(const std::string& name, const float value) const override;
  void setFloatArr(const std::string& name, const float* data,
                   int count) const override;
  void setVec2(const std::string& name, const glm::vec2& value) const override;
  void setVec3(const std::string& name, const glm::vec3& value) const override;
  void setVec3Arr(const std::string& name, const glm::vec3* data,
                  int count) const override;
  void setMat3(const std::string& name, const glm::mat3& value) const override;
  void setMat4(const std::string& name, const glm::mat4& value) const override;

 private:
  unsigned int shader_program_ = 0;
  std::unordered_map<GLuint, GlShader> shaders_;
};
}  // namespace gpu
