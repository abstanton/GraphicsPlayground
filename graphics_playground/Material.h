#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "gpu/GPUShader.h"
#include "resources/Texture.h"

/*
        Represents the following glsl:
        uniform bool name_use_tex;
        uniform T name_val;
        uniform sampler2D name_tex;
        uniform vec2 name_scale;

*/
using std::string;

template <typename T>
struct TextureOr {
  T value;
  bool use_tex = false;
  glm::vec2 scale;
  string tex_name;

  void setValue(T val) {
    use_tex = false;
    value = val;
  }

  void setValue(string tex, glm::vec2 tex_scale = {1, 1}) {
    use_tex = true;
    tex_name = tex;
    scale = tex_scale;
  }
};

/*
        Represents the following glsl:
        uniform T name;
*/
template <typename T>
struct Uniform {
  T value;

  void setValue(T val) { value = val; }
};

using std::string;

struct MaterialComp {
  void setColourInput(string name, glm::vec3 value,
                      glm::vec2 tex_scale = {1, 1}) {
    if (colour_inputs.find(name) == colour_inputs.end()) {
      return;
    }
    colour_inputs[name].setValue(value);
  }

  void setColourInput(string name, string texture_name,
                      glm::vec2 tex_scale = {1, 1}) {
    if (colour_inputs.find(name) == colour_inputs.end()) {
      return;
    }
    colour_inputs[name].setValue(texture_name, tex_scale);
  }

  void setFloatInput(string name, float value, glm::vec2 tex_scale = {1, 1}) {
    if (float_inputs.find(name) == float_inputs.end()) {
      return;
    }
    float_inputs[name].setValue(value);
  }

  void setFloatInput(string name, string texture_name,
                     glm::vec2 tex_scale = {1, 1}) {
    if (float_inputs.find(name) == float_inputs.end()) {
      return;
    }
    float_inputs[name].setValue(texture_name, tex_scale);
  }

  std::map<string, TextureOr<glm::vec3>> colour_inputs;
  std::map<string, TextureOr<float>> float_inputs;

  std::map<string, Uniform<glm::vec3>> colour_uniforms;
  std::map<string, Uniform<float>> float_uniforms;
  std::map<string, Uniform<bool>> bool_uniforms;

  std::string shader_name;
  bool transparent;
  bool receive_shadows;
};

class MaterialBuilder {
 public:
  MaterialBuilder() {}

  MaterialBuilder& addColourInput(string name) {
    material_.colour_inputs[name] = TextureOr<glm::vec3>{};
    return *this;
  }
  MaterialBuilder& addFloatInput(string name) {
    material_.float_inputs[name] = TextureOr<float>{};
    return *this;
  }
  MaterialBuilder& addColourUniform(string name) {
    material_.colour_uniforms[name] = Uniform<glm::vec3>{};
    return *this;
  }
  MaterialBuilder& addFloatUniform(string name) {
    material_.float_uniforms[name] = Uniform<float>{};
    return *this;
  }
  MaterialBuilder& addBoolUniform(string name) {
    material_.bool_uniforms[name] = Uniform<bool>{};
    return *this;
  }

  MaterialComp build() { return material_; }

 private:
  MaterialComp material_;
};

class Material {
 public:
  float tex_scale = 1.0f;
  void setModelMatrix(const glm::mat4 model) const;

  virtual std::vector<const Texture*> getTextures() const = 0;
  std::shared_ptr<gpu::Shader> shader_;
};
