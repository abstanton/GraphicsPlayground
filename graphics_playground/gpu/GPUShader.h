#pragma once
#include <glm/glm.hpp>
#include <string>

#include "GPUTexture.h"

namespace gpu {
class Shader {
 public:
  virtual void use() const = 0;
  virtual void setTexture(const char* name,
                          const gpu::Texture* texture) const = 0;
  virtual void setBool(const std::string& name, const bool value) const = 0;
  virtual void setInt(const std::string& name, const int value) const = 0;
  virtual void setFloat(const std::string& name, const float value) const = 0;
  virtual void setFloatArr(const std::string& name, const float* data,
                           int count) const = 0;
  virtual void setVec2(const std::string& name,
                       const glm::vec2& value) const = 0;
  virtual void setVec3(const std::string& name,
                       const glm::vec3& value) const = 0;
  virtual void setVec3Arr(const std::string& name, const glm::vec3* data,
                          int count) const = 0;
  virtual void setMat3(const std::string& name,
                       const glm::mat3& value) const = 0;
  virtual void setMat4(const std::string& name,
                       const glm::mat4& value) const = 0;
};
}  // namespace gpu
